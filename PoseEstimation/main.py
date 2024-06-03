# CSM Bahar Flagbot Project - Human Pose Detection
# Author: Charlie Hagen

# When run, the sript will create a window with the active capture. To exit, press q.

import mediapipe as mp
import cv2
import numpy as np
import sys
import autocorrect as ac
import time
import serial

#Angle between three points denormalized
def calculate_angle(a,b,c):
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)

    rad = np.arctan2(c[1]-b[1], c[0]-b[0]) - np.arctan2(a[1]-b[1], a[0]-b[0])
    angle = (rad * 180.0 / np.pi) 

    if(angle < 0):
        angle = angle + 360
    return angle

#Absolute angle between three points
def calculate_angle_norm(a,b,c):
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)

    rad = np.arctan2(c[1]-b[1], c[0]-b[0]) - np.arctan2(a[1]-b[1], a[0]-b[0])
    angle = np.abs(rad * 180.0 / np.pi) 
    if(angle > 180):
        angle = 360 - angle

    return angle 

#Look up table for semaphore 
def semaphore_letter(left_angle, right_angle):
    #Round the angles to the nearest 45 degrees
    left = (round(left_angle / 45.0) * 45.0) % 360
    right = (round(right_angle / 45.0) * 45.0) % 360
    if ((left,right) in semaphore):
        letter = semaphore[(left,right)] #Opposite way because the robot faces the human - EDIT: now mirroed, not reversed 5/22/24
    else:
        letter = "Invalid"
    return letter

# intakes a letter and returns a tuple of angles (left,right)
def get_semaphore_angles(letter):
    return reverse_semaphore.get(letter)

'''
All Command Methods
'''
# in this method, the robot signs 'Hello' back
def hi_command():
    send_command(get_semaphore_angles('H'))
    send_command(get_semaphore_angles('E'))
    send_command(get_semaphore_angles('L'))
    send_command(get_semaphore_angles('L'))
    send_command(get_semaphore_angles('O'))
    send_command(get_semaphore_angles('End of Word'))
    
# this method sends commands to the Arduino to move the stepper motors
# command format: "left_motor_degrees,right_motor_degrees"
# ex: "35,50"
# 'command' is a tuple of the two angles
def send_command(command):
    # First, take the tuple and turn it into a string
    command_string_representation = ",".join(map(str, command))

    # now send that to the arduino!
    usb.write(command_string_representation.encode('utf-8'))
    
# this method validates the command the human wrote and runs the corresponding method
def run_command(word):
    # run the autocorrect
    word = autocorrect(word)

    # make it all lowercase
    word = word.lower()

    # ensure the word is a real command
    if word not in command_dict:
        # do something to indicate the command is not in the command list
        # remove the print statement once we can do something else useful
        print("Command not in command list!")
        return
    else:
        # figure out which valid word it was and call that method
        if word == 'hi':
            hi_command()


'''
End Command Methods
'''

'''
Initialization
'''
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

# create a dictionary based on the words we want to use
# word maps to frequency
command_dict = {
    "dance":0.25,
    "act":0.25,
    "hi":0.25,
    "hello":0.25
}

# make the autocorrect use our dictionary
autocorrect = ac.Speller(nlp_data=command_dict)

try:
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
except Exception as e:
    print(f"An error occurred: {e}")
    sys.exit(1)  # Exit with a non-zero status code

#Pull resolution
resx = int(cap.get(3))
resy = int(cap.get(4))

#Set up counters for write logic 
letter_count = 0
letter_time = time.perf_counter()
previous_letter = ""
written_string = ""
letter_selected = False
letter_written = False

TIME_TO_SELECT = 2
TIME_TO_RESET = 0.5

#Setup Arudino Serial connection
# if not correct, run "$ ls /dev/tty*"
USB_PORT = "/dev/ttyACM0"

try:
   usb = serial.Serial(USB_PORT, 9600, timeout=2)
except:
   print("ERROR - Could not open USB serial port.  Please check your port name and permissions.")
   
#Semaphore lookup table
semaphore = {
        (45, 0) : "A",
        (90, 0) : "B",
        (135, 0) : "C",
        (180, 0) : "D",
        (0, 135) : "E",
        (0, 90) : "F",
        (0, 45) : "G",
        (90, 315) : "H",
        (135, 315) : "I",
        (180, 90) : "J",
        (45, 180) : "K",
        (45, 135) : "L",
        (45, 90) : "M",
        (45, 45) : "N",
        (135, 270) : "O",
        (90, 180) : "P",
        (90, 135) : "Q",
        (90, 90) : "R",
        (90, 45) : "S",
        (135, 180) : "T",
        (135, 135) : "U",
        (180, 45) : "V",
        (270, 135) : "W",
        (315, 135) : "X",
        (135, 90) : "Y",
        (315, 90) : "Z",
        (180, 180) : "End of Word",
    }

# Create a reverse lookup dictionary
# reverse_semaphore = {v: k for k, v in semaphore.items()}

# by hand
# H is weird
reverse_semaphore = {
    "E" : (-135, 0),
    "F" : (-90, 0),
    "H" : (45, 90),
    "L" : (-135, 45),
    "N" : (-45, 45),
    "O" : (-225, 90),
    "P" : (-180, 90),
    "R" : (-90, 90),
    "T" : (-180, 135),
    "U" : (-135,135),
    "W" : (-90, 225),
    "Y" : (-90, 135),
    "End of Word" : (-180, 180),
}

'''
End Initalization
'''

'''
Video Feed Loop
'''
with mp_pose.Pose(min_detection_confidence = 0.9, min_tracking_confidence=0.8) as pose: #Set detection and tracking confidence threshholds 
    while cap.isOpened():
        ret, frame = cap.read()

        #Recolor image into RGB format
        frame = cv2.flip(frame, 1)
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False

        #Process the image, detect pose
        results = pose.process(image)

        #Recolor image into BGR format
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        #Extract the landmarks
        try:
            landmarks = results.pose_landmarks.landmark
            left_shoulder = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x,landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y]
            left_elbow = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].x,landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y]
            left_wrist = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].x,landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y]
            left_hip = [landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].x,landmarks[mp_pose.PoseLandmark.LEFT_HIP.value].y] 

            right_shoulder = [landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].y]
            right_elbow = [landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].y]
            right_wrist = [landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].y]
            right_hip = [landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].x,landmarks[mp_pose.PoseLandmark.RIGHT_HIP.value].y] 

            center = [(right_hip[0] + 2*right_shoulder[0] + 2*left_shoulder[0] + left_hip[0])/6, (right_hip[1] + 2*right_shoulder[1] + 2*left_shoulder[1] + left_hip[1])/6]

            #Calculate relevant angles
            left_shoulder_angle = calculate_angle(left_hip, left_shoulder, left_wrist)
            left_elbow_angle = calculate_angle_norm(left_shoulder, left_elbow, left_wrist)
            
            right_shoulder_angle = calculate_angle(right_hip, right_shoulder, right_wrist)
            right_elbow_angle = calculate_angle_norm(right_shoulder, right_elbow, right_wrist)


            #Calculate if the person is facing the camera
            if(right_shoulder[0] < left_shoulder[0]):
                left_shoulder_angle = 360 - left_shoulder_angle #Modify the left shoulder angle
                cv2.putText(image, "Facing Camera", (0,resy), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)
            else:
                right_shoulder_angle = 360 - right_shoulder_angle #Modify the right shoulder angle
                cv2.putText(image, "Facing Away", (0,resy), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)

            #Get active letter
            letter = semaphore_letter(left_shoulder_angle, right_shoulder_angle)

            #Check if the elbows are extended
            if(left_elbow_angle > 140 and right_elbow_angle > 140 ):
                cv2.putText(image, "Arms: extended", (0,resy-50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)
                arms_extended = True
                letter_color = (0,255,255)
            else:
                cv2.putText(image, "Arms: not extended", (0,resy-50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)
                arms_extended = False
                letter_color = (0,0,255)

            if(arms_extended):
                print(letter_time)
                if(letter == "Invalid" or previous_letter != letter):
                    letter_time = time.perf_counter()
                    letter_selected = False
                    letter_written = False
                if(time.perf_counter() - letter_time >= TIME_TO_SELECT):
                    if(not letter_selected):
                        letter_color = (0,255,0)
                        letter_selected = True
                        if(letter == "End of Word"):
                            # since the word is done being written, run the requested command
                            run_command(written_string)
                        else:
                            written_string += letter
                            letter_written = True
                    else:
                        #Only show green, no action
                        letter_color = (0,255,0)
                if(time.perf_counter() - letter_time >= TIME_TO_SELECT + TIME_TO_RESET and letter_selected):
                    #Reset after selected letter
                    letter_time = time.perf_counter()
                    letter_selected = False
                    letter_written = False
            else:
                letter_time = time.perf_counter()  
                letter_selected = False
                letter_written = False

            #Current string to be rendered to the screen
            cv2.putText(image, written_string, (0,resy-100), cv2.FONT_HERSHEY_SIMPLEX, 2.5, (255,100,0), 2, cv2.LINE_AA)

            #Render the detected landmarks
            mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

            #Left arm lines
            cv2.line(image, tuple(np.multiply(left_shoulder, [resx, resy]).astype(int)), tuple(np.multiply(left_wrist, [resx, resy]).astype(int)),(255,0,0), 3)
            cv2.line(image, tuple(np.multiply(left_shoulder, [resx, resy]).astype(int)), tuple(np.multiply(left_hip, [resx, resy]).astype(int)),(255,0,0), 3)
            
            # Right arm lines
            cv2.line(image, tuple(np.multiply(right_shoulder, [resx, resy]).astype(int)), tuple(np.multiply(right_wrist, [resx, resy]).astype(int)),(0,0,255), 3)
            cv2.line(image, tuple(np.multiply(right_shoulder, [resx, resy]).astype(int)), tuple(np.multiply(right_hip, [resx, resy]).astype(int)),(0,0,255), 3)

            #Angle text
            cv2.putText(image, "{:.1f}".format(left_shoulder_angle), tuple(np.multiply(left_shoulder, [resx, resy]).astype(int)),cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)
            cv2.putText(image, "{:.1f}".format(right_shoulder_angle), tuple(np.multiply(right_shoulder, [resx, resy]).astype(int)),cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 2, cv2.LINE_AA)
            
            text_size = cv2.getTextSize(letter, cv2.FONT_HERSHEY_SIMPLEX, 1.5, 2)
            letter_coordinates = (int(resx * center[0] - text_size[0][0]/2), int(resy * center[1] - text_size[0][1]/2))
            cv2.putText(image, letter, letter_coordinates,cv2.FONT_HERSHEY_SIMPLEX, 1.5, letter_color, 2, cv2.LINE_AA)
            
            previous_letter = letter

        except:
            pass

        cv2.imshow('Mediapipe Feed', image)
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

