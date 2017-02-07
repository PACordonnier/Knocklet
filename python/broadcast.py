from bluepy.btle import Scanner, DefaultDelegate, Peripheral
from uuid import getnode as get_mac
import simplejson
import json
import requests

url = "http://jeedom.local/plugins/knocklet/core/api/knocklet.api.php"
headers = {'content-type': 'application/json'}

def postDatas(deviceAddr, knocks, rssi):
    payload = {
      "method": "knock",
      "jsonrpc": "2.0",
      "params": {
            "apikey": "lbippE2VJkuxVdfI4sKtZWQkCTwNJxH517kYVYEDLBbUe2eD",
            "braceletId":1,
            "moduleId":1,
      }
    }
    if(knocks == 'ff'):
        payload["method"] = 'init'
    else:
        payload["params"]["knocks"] = knocks
        payload["params"]["rssi"] = str(rssi)
    payload["params"]["moduleId"] = str(get_mac())
    payload["params"]["braceletId"] = deviceAddr.encode('utf-8')
    print "Sending  datas" + knocks
    print payload
    data = json.dumps(payload)
    requests.post(url, data = data, headers=headers).json()
    print "data sent"

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if (dev.addr == "4b:4e:4f:00:00:02" or dev.addr == "4b:4e:4f:00:00:01"): 
            if isNewDev:
                print "Discovered device", dev.addr
                knocks =  dev.getValueText(33)[-2:].encode('utf-8')
                deviceAddr = dev.addr
                scanner.clear()
                postDatas(deviceAddr, knocks, dev.rssi) 

scanner = Scanner().withDelegate(ScanDelegate())
scanner.start()
scanner.process(0)

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
