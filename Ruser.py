import pickle
import pprint 


USERDATABIN_FILE_PATH ="./UserData.bin"
# PARKINGDATABIN_FILE_PATH = "./ParkingSpaceData.bin"

pp = pprint.PrettyPrinter(indent=4)

def read_binary_file(filepath):
  with open(filepath, "rb") as file:
    result = pickle.load(file)
    pp.pprint(result)

read_binary_file(USERDATABIN_FILE_PATH)
# read_binary_file(PARKINGDATABIN_FILE_PATH)