import serial
import time

def send_command(command):
     # First, take the tuple and turn it into a string
    command_string_representation = ",".join(map(str, command))

    # now send that to the arduino!
    usb.write(command_string_representation.encode('utf-8'))

     # now wait for confirmation from the arudino that the arm is in the correct position
    ack = "not ready"
    while(ack != "Arms in place"):
       ack = usb.readline().decode("utf-8").strip()


# Main

USB_PORT = "/dev/ttyACM0"  # Arduino Uno WiFi Rev2

# Connect to USB serial port at 9600 baud
# deleted timeout parameter
try:
   usb = serial.Serial(USB_PORT, 9600)
except:
   print("ERROR - Could not open USB serial port.  Please check your port name and permissions.")
   print("Exiting program.")
   exit()

# delay to let the arduino finish setting up
time.sleep(3)

# now make the robot sign "TEST"
send_command("-180, 135") # T
send_command("-135, 0") # E
send_command("-90, 45") # S
send_command("-180, 135") # T

