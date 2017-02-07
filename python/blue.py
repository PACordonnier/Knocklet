from bluepy.btle import Scanner, DefaultDelegate, Peripheral
import requests
import json
import struct

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print "Discovered device", dev.addr
        elif isNewData:
            print "Received new data from", dev.addr

scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(0.1)

macAddress = "4b:4e:4f:00:00:01"

for dev in devices:
    print "Device %s (%s), RSSI=%d dB" % (dev.addr, dev.addrType, dev.rssi)
    for (adtype, desc, value) in dev.getScanData():
        print "  %s = %s" % (desc, value)

periph = Peripheral(macAddress)
services = periph.getServices()
for service in services:
	print "Service UUID : %s" % (service.uuid.getCommonName())
	characteristics = service.getCharacteristics()
	for char in characteristics:
		print "UUID : %s, property : %s" % (char.uuid.getCommonName(), char.propertiesToString())
		if (char.uuid.getCommonName() == "626f6972-6564-656c-616c-636f6f6c0100"):
			knocks = struct.unpack("<b", char.read())[0]
			print knocks
periph.disconnect()

url = "http://192.168.0.10/plugins/knocklet/core/api/knocklet.api.php"
headers = {'content-type': 'application/json'}

payload = {
  "method": "knock",
  "jsonrpc": "2.0",
  "params": {
        "apikey": "lbippE2VJkuxVdfI4sKtZWQkCTwNJxH517kYVYEDLBbUe2eD",
        "id":8,
        "braceletId":1,
        "moduleId":1,
        "knocks":1
  }
}
payload['params']['braceletId'] = macAddress
payload['params']['knocks'] = knocks
print json.dumps(payload)
response = requests.post(url, data = json.dumps(payload), headers=headers).json()
print payload
