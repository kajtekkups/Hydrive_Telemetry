# import json
# import threading
# import time
#
# # Define the data we want to write to the JSON file
# data = {'name': 'John', 'age': 30, 'city': 'New York'}
#
# # Define a function that will write the data to a JSON file
# def write_data():
#     while True:
#         with open('data.json', 'w') as f:
#             json.dump(data, f)
#         time.sleep(1)  # sleep for 1 second before writing again
#
# # Define a function that will read the data from the JSON file
# def read_data():
#     while True:
#         with open('data.json', 'r') as f:
#             new_data = json.load(f)
#             print(new_data)
#         time.sleep(5)  # sleep for 5 seconds before reading again
#
# # Create and start the two threads
# write_thread = threading.Thread(target=write_data)
# read_thread = threading.Thread(target=read_data)
# write_thread.start()
# read_thread.start()
#
# # Wait for the threads to finish (which they won't, since they run indefinitely)
# write_thread.join()
# read_thread.join()

# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0.


from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import sys


myMQTTClient = AWSIoTMQTTClient("Hydrive_komputer")

myMQTTClient.configureEndpoint("andh08iyq0pvi-ats.iot.eu-north-1.amazonaws.com", 443)
myMQTTClient.configureCredentials("./certyfikaty_komp/AmazonRootCA1.pem", "./certyfikaty_komp/private.pem.key", "./certyfikaty_komp/certificate.pem.crt.txt")

myMQTTClient.connect()
print("Client Connected")

topic = "Hydrive_komputer_sub"





def handle_message(client, userdata, message):
    print(f"Received message: {message.payload.decode('utf-8')} on topic {message.topic}")


myMQTTClient.subscribe(topic, 0, handle_message)
print("Message Sent")

while True:
    pass