import requests
import json


def main():
    url = "http://jeedom.fr/core/api/jeeApi.php"
    headers = {'content-type': 'application/json'}

    # Example echo method
    payload = {
        "method": "version",
        "jsonrpc": "2.0",
        "params": {
                "apikey": "lbippE2VJkuxVdfI4sKtZWQkCTwNJxH517kYVYEDLBbUe2eD"
        }
    }
    response = requests.post(
            url, data=json.dumps(payload), headers=headers).json()

    print response
if __name__ == "__main__":
    main()
