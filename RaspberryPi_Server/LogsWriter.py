

class Logs:

    def clean_log(self):
        file_log = open("debug/logs.txt", "w")
        file_log.close()
    def write_log(self, string):
        file_log = open("debug/logs.txt", "a")
        file_log.write(string+"\n")
        file_log.close()