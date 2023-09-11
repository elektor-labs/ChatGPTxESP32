# About the code

# This Python code is designed to run on a microcontroller, such as an ESP32, and perform the following tasks:
# 1. Connect to a Wi-Fi network using the provided SSID and password.
# 2. Blink an LED to provide visual feedback of successful Wi-Fi connection.
# 3. Create a socket server on port 80 to listen for incoming connections.
# 4. Accept incoming connections from a remote device (e.g., another ESP32).
# 5. Receive a code snippet from the remote device.
# 6. Execute the received code snippet.
# 7. Send a response back to the remote device, indicating success or error.
#
# Instructions for usage:
# - Set "wifi_ssid" and "wifi_password" to your Wi-Fi network credentials.
# - Upload the code to your microcontroller.
# - Ensure the microcontroller is connected to the Wi-Fi network.
# - The code will blink an LED to indicate successful Wi-Fi connection.
# - The microcontroller will act as a socket server on port 80, waiting for incoming connections.
# - You can send code snippets to the microcontroller from a remote device.
# - The microcontroller will execute the received code and send a response back to the remote device.
# - This code is intended for basic remote code execution and interaction with microcontroller-based systems.
# - This code is not intended for use in production systems.

import network
import socket
import machine
import time

# Wi-Fi credentials
wifi_ssid = "xxx"
wifi_password = "xxx"

# Connect to Wi-Fi
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(wifi_ssid, wifi_password)

# Wait until connected to Wi-Fi
while not wifi.isconnected():
    pass

# Print the Wi-Fi connection details
print("Connected to Wi-Fi")
print("IP Address:", wifi.ifconfig()[0])


# Define the LED pin
led_pin = machine.Pin(2, machine.Pin.OUT)
i = 1
while i < 2:
    led_pin.on()
    time.sleep(0.5)
    led_pin.off()
    time.sleep(0.5)
    i += 1


# Function to blink the LED
def blink_led():
    i = 1
    while i < 4:
        led_pin.on()
        time.sleep(0.3)
        led_pin.off()
        time.sleep(0.3)
        i += 1

blink_led()

# Create a socket server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('', 80))
server.listen(1)

# Accept and handle incoming connections
while True:
    print("Waiting for connection...")
    client, addr = server.accept()
    print("Client connected:", addr)
    
    # Receive the code snippet from the first ESP32
    code = ""
    while True:
        data = client.recv(1024)
        if not data:
            break
        code += data.decode()
    
    # Execute the received code
    try:
        print(code)
        exec(code)
        response = "Code executed successfully"
        print (response)
    except Exception as e:
        response = "Error executing code: " + str(e)
    
    # Send the response back to the first ESP32
    client.sendall(response.encode())
    
    # Close the connection
    client.close()
    print("Client disconnected")
