from bluepy.btle import Scanner, DefaultDelegate, Peripheral
from uuid import getnode as get_mac
import json
import requests

url = "http://jeedom.local/plugins/knocklet/core/api/knocklet.api.php"
headers = {'content-type': 'application/json'}

payload = {
  "method": "knock",
  "jsonrpc": "2.0",
  "params": {
        "apikey": "lbippE2VJkuxVdfI4sKtZWQkCTwNJxH517kYVYEDLBbUe2eD",
        "id":8,
        "braceletId":1,
        "moduleId":1,
        "knocks":1,
        "rssi":1
  }
}
payload["params"]["moduleId"] = get_mac()
print payload

def postDatas(deviceAddr, knocks, rssi):
    payload["params"]["knocks"] = knocks
    payload["params"]["braceletId"] = deviceAddr
    payload["params"]["rssi"] = rssi
    requests.post(url, data = json.dumps(payload), headers=headers).json()

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if (dev.addr == "4b:4e:4f:00:00:02" or dev.addr == "4b:4e:4f:00:00:01"): 
            if isNewDev:
                print "Discovered device", dev.addr
                knocks =  dev.getValueText(33)[-2:]
                deviceAddr = dev.addr
                scanner.clear()
                postDatas(deviceAddr, knocks, dev.rssi) 

scanner = Scanner().withDelegate(ScanDelegate())
scanner.start()
scanner.process(0)

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
