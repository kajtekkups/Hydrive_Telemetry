import json
import gspread
from google.oauth2.service_account import Credentials
import os

class GoogleSheetsApi:
    def __init__(self):
        self.certificate = 'C:/Users/studia/Desktop/Hydrive/Hydrive_Telemetry/MQTT_Python/esp32podejscie3-22b84f388c92.json'

    def polacz_z_google(self, sheetName):
        # Uwierzytelnianie za pomocą pliku JSON z kluczem usługi Google
        scopes = ['https://www.googleapis.com/auth/spreadsheets', 'https://www.googleapis.com/auth/drive']
        creds = Credentials.from_service_account_file(self.certificate, scopes=scopes)
        client = gspread.authorize(creds)

        # Otwórz arkusz Google za pomocą jego nazwy
        self.sheet = client.open(sheetName).sheet1

    #row - tablica z wartosciami  row = [data[key] for key in sorted(data.keys())]
    def wyslij_w_chmure(self, row):
        self.sheet.append_row(row)


# while True:
#     # Odczytaj dane z pliku JSON
#     try:
#         file_path = 'C:/Users/studia/Desktop/Hydrive/Hydrive_Telemetry/MQTT_Python/data.json'
#         with open('C:/Users/studia/Desktop/Hydrive/Hydrive_Telemetry/MQTT_Python/data.json') as json_file:
#             data = json.load(json_file)
#
#         # Wysyłanie danych do Google Sheets
#         # Zakładamy, że dane to lista list, gdzie każda lista to jeden wiersz do dodania
#
#         try:
#             os.remove(file_path)
#             print("File deleted successfully.")
#         except OSError as error:
#             print(f"Error: {file_path} - {error}")
#
#     except:
#         print("nie ma takiego pliku, czekam az powstanie")

