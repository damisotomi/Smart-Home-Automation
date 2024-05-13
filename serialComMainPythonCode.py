import cv2
import serial
from module import findnameoflandmark, findpostion

# Initialize serial communication with Arduino
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

# Set up the video capture
cap = cv2.VideoCapture(0)
tip = [8, 12, 16, 20]
tipname = [8, 12, 16, 20]
finger_binary_dict = {8: 1, 12: 10, 16: 100, 20: 1000}	#dictionary for detected fingers

while True:
    ret, frame = cap.read()
    frame1 = cv2.resize(frame, (640, 480))
    #fill in list of position and landmarks
    a = findpostion(frame1)
    b = findnameoflandmark(frame1)
    
    if len(b and a) != 0:
        fingersum = 0
        for id in range(0, 4):
            if a[tip[id]][2:] < a[tip[id]-2][2:]:	#compare position of finger tip to position of second preceding index 
                fingersum += finger_binary_dict[tipname[id]]	#sum up to send fingersum to arduino using dictionary values
        
        fingersum_int = int(str(fingersum), 2)
        
        print("Integer =", fingersum_int)
        
        # Send fingersum to Arduino
        ser.write((str(fingersum_int) + '\n').encode())

    cv2.imshow("Frame", frame1)
    key = cv2.waitKey(1) & 0xFF

    if key == ord("s"):	#break out of loop by pressing s key on keyboard
        break

cap.release()
cv2.destroyAllWindows()
ser.close()
