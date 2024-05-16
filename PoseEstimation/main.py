# CSM Bahar Flagbot Project - Human Pose Detection
# Author: Charlie Hagen

# When run, the sript will create a window with the active capture. To exit, press q.

import mediapipe as mp
import cv2
import numpy as np
import sys

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
        (0, 0) : "End of Word",
    }
    if ((right,left) in semaphore):
        letter = semaphore[(right,left)] #Opposite way because the robot faces the human
    else:
        letter = "Invalid"
    return letter


#Initialization
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

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
previous_letter = ""
written_string = ""

#Video Feed
with mp_pose.Pose(min_detection_confidence = 0.9, min_tracking_confidence=0.8) as pose: #Set detection and tracking confidence threshholds 
    while cap.isOpened():
        ret, frame = cap.read()

        #Recolor image into RGB format
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
                if(letter == "Invalid" or letter == "End of Word"):
                    letter_count = 0
                if(previous_letter == letter):
                    letter_count += 1
                else:
                    letter_count = 0
                if(letter_count == 50):
                    letter_color = (0,255,0)
                    written_string += letter
                if(letter_count > 50 and letter_count < 70):
                    letter_color = (0,255,0)
                if(letter_count >= 70):
                    letter_count = 0
            else:
                letter_count = 0

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

