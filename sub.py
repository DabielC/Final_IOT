import paho.mqtt.client as mqtt
import RPi.GPIO as gp

gp.setmode(gp.BOARD)
LED_PIN = 7
gp.setup(LED_PIN, gp.OUT)

# PWM for LED
pwm = gp.PWM(LED_PIN, 1000)  
pwm.start(0)  # Start with LED off

MQTT_BROKER = 'phycom.it.kmitl.ac.th'
MQTT_PORT = 1883
MQTT_TOPIC = '65070009'

# Variable to track LED state
led_state = False  # False = OFF, True = ON

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, message):
    global led_state
    payload = message.payload.decode().strip()
    print(f"Received: '{payload}'")

    if payload.upper() == "ON":
        pwm.ChangeDutyCycle(100)  # LED Full Bright
        led_state = True  # Allow brightness adjustment
    elif payload.upper() == "OFF":
        pwm.ChangeDutyCycle(0)  # LED OFF
        led_state = False  # Stop accepting brightness values
    elif payload.isdigit() and led_state:  
        brightness = max(0, min(100, int(payload)))  # Ensure 0-100 range
        pwm.ChangeDutyCycle(brightness)  
    elif payload.isdigit() and not led_state:
        print("Ignored brightness command because LED is OFF")
    else:
        print("Invalid command received")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_start()

try:
    while True:
        pass
except KeyboardInterrupt:
    print("Exiting...")
    client.loop_stop()
    client.disconnect()
    pwm.stop()
    gp.cleanup()
