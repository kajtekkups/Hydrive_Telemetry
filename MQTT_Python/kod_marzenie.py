from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import json
import gspread
from google.oauth2.service_account import Credentials

import time

class GoogleSheetsApi:
    def __init__(self):
        self.certificate = 'C:/Users/studia/Desktop/Hydrive/Hydrive_Telemetry/MQTT_Python/esp32podejscie3-22b84f388c92.json'
        self.last_written_row = None

    def polacz_z_google(self, sheetName):
        # Uwierzytelnianie za pomocą pliku JSON z kluczem usługi Google
        scopes = ['https://www.googleapis.com/auth/spreadsheets', 'https://www.googleapis.com/auth/drive']
        creds = Credentials.from_service_account_file(self.certificate, scopes=scopes)
        client = gspread.authorize(creds)

        # Otwórz arkusz Google za pomocą jego nazwy
        self.sheet = client.open(sheetName).sheet1

        # Get the values of all rows in the sheet
        rows = self.sheet.get_all_values()
        # Find the last non-empty row in the sheet
        last_row = len(rows)
        for i in range(len(rows) - 1, -1, -1):
            if not all(val == '' for val in rows[i]):
                last_row = i + 1
                break
        self.last_written_row = last_row + 1


    #row - tablica z wartosciami  row = [data[key] for key in sorted(data.keys())]
    def wyslij_w_chmure(self, rows):
        # self.sheet.append_row(row)
        #update rows where data is written
        row_number = len(rows)
        first_row = self.last_written_row
        self.last_written_row += row_number

        self.sheet.update(f'A{first_row}:H{self.last_written_row}', rows)

#Ustawienia serwera aws
myMQTTClient = AWSIoTMQTTClient("Hydrive_komputer")
myMQTTClient.configureEndpoint("andh08iyq0pvi-ats.iot.eu-north-1.amazonaws.com", 443)
myMQTTClient.configureCredentials("./certyfikaty_komp/AmazonRootCA1.pem", "./certyfikaty_komp/private.pem.key", "./certyfikaty_komp/certificate.pem.crt.txt")
myMQTTClient.connect()
print("Client Connected")
topic = "Hydrive_komputer_sub"

#inicjalizacja google sheets
google_sheet = GoogleSheetsApi()
prosty_licznik = 0
dane_do_wyslania = []

def handle_message(client, userdata, message):

    # convert message payload to character buffer
    json_str = message.payload.decode('utf-8')

    # deserialize JSON
    json_obj = json.loads(json_str)

    # access JSON data
    data = [str(json_obj["time"]), str(json_obj["Temp"]), str(json_obj["pomiarVT_0"]), str(json_obj["pomiarVT_1"]), str(json_obj["pomiarV_2"]), str(json_obj["pomiarI_0"]),
            str(json_obj["pomiarI_1"]), str(json_obj["pomiarI_2"])]

    print(data[0])

    #przesylanie danych do google sheet
    global prosty_licznik
    global dane_do_wyslania

    number_of_data_samples = 6
    if prosty_licznik == number_of_data_samples:
        try:
            google_sheet.wyslij_w_chmure(dane_do_wyslania)
            dane_do_wyslania = []
            prosty_licznik = 0
        except Exception as e:
            print("An error occurred:", e)

    else:
        prosty_licznik +=1
        dane_do_wyslania.append(data)

if __name__ == "__main__":
    google_sheet.polacz_z_google('Hydrive_Telemetria')
    myMQTTClient.subscribe(topic, 0, handle_message)
    print("Message Sent")

    while True:

        pass