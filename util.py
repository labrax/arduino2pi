
import requests

class InfluxHandler:
    def __init__(self, address, database, table, host_sender):
        import requests
        self.address = address
        self.database = database
        self.table = table
        self.host = host_sender
        self.url_string = 'http://{address}/write?db={database}'.format(self.address, self.database)

    def _formatMessage(self, sensors):
        return '{table_name},host={host} {message}'.format(table_name=self.table, host=self.host, message=','.join(["{}={}" for i, j in sensors]))

    def sendInflux(self, sensors):
        print(self.url_string, _formatMessage(sensors))
        #return requests.post(self.url_string, data=_formatMessage(sensors))


