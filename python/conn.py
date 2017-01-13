import requests
import json

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

response = requests.post(url, data = json.dumps(payload), headers=headers).json()
print response
