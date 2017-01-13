from bluepy.btle import Scanner, DefaultDelegate, Peripheral

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
		print "Discovered device", dev.addr
		print dev.getScanData()
        elif isNewData:
		print "Received new data from", dev.addr
		print dev.getScanData()

scanner = Scanner().withDelegate(ScanDelegate())
scanner.start()
scanner.process(30)
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
