import requests
from time import*
from wiringpi import Serial


baud = 9600
ser  = Serial("/dev/serial0",baud)
sleep(0.3)

TOKEN = "BBFF-8QJN2VbmRg7ZrSwlisoJgYqLaH4YsM"  # Put your TOKEN here
DEVICE_LABEL = "InvernaderoProyecto"  # Put your device label here 
VARIABLE_LABEL_1 = "temperatura"  # Put your first variable label here
VARIABLE_LABEL_2 = "humedad"  # Put your second variable label here
VARIABLE_LABEL_3 = "luminosidad"  # Put your second variable label here

def recibir(echo = True):
 data = ""
 while True:
  input = ser.getchar()
  if echo:
   ser.putchar(input)
  if input == "\r":
   return (data)
  data += input
 sleep(0.2)
 
def build_payload(variable_1, variable_2, variable_3, temperatura, humedad, luz):
    print ("CONECTADO...")  
    payload = {variable_1: temperatura,
              variable_2: humedad,
              variable_3: luz}

    return payload
   
def post_request(payload):
    # Creates the headers for the HTTP requests
    url = "http://industrial.api.ubidots.com"
    url = "{}/api/v1.6/devices/{}".format(url, DEVICE_LABEL)
    headers = {"X-Auth-Token": TOKEN, "Content-Type": "application/json"}

    # Makes the HTTP requests
    status = 400
    attempts = 0
    while status >= 400 and attempts <= 5:
        req = requests.post(url=url, headers=headers, json=payload)
        status = req.status_code
        attempts += 1
        sleep(1)

    # Processes results
    if status >= 400:
        print("[ERROR] Could not send data after 5 attempts, please check \
            your token credentials and internet connection")
        return False

    print("[INFO] request made properly, your device is updated")
    return True

while(1): 
    mensaje = recibir()
    vals = mensaje.split(',')
    sleep(0.2)
    temperatura = vals[1]
    humedad = vals[0]
    luz = vals[2]
    payload = build_payload(
    VARIABLE_LABEL_1, VARIABLE_LABEL_2, VARIABLE_LABEL_3,temperatura, humedad, luz)
    print("[INFO] Attemping to send data")
    post_request(payload)
    print("[INFO] finished")
    
