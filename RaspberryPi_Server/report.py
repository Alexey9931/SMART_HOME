import time, datetime
import mysql.connector
from fpdf import FPDF
import LogsWriter

#Параметры локальной БД в phpmyadmin
HOST = "localhost"  #Имя хоста
USER = "root"   #Имя пользователя в phpmyadmin
PASSWORD = "al281299" #Пароль в phpmyadmin
DATABASE = "esp_data"   #Имя базы данных в phpmyadmin

class MakeReport:

    header = {"Content-type": "application/x-www-form-urlencoded"}
    street_temp = ""
    street_hum = ""
    home_temp = ""
    home_hum = ""
    pressure = ""
    wind_speed = ""
    wind_direct = ""
    rain = ""
    cur_date = datetime.datetime.now()

    street_temp_morning = 0.0
    street_temp_day = 0.0
    street_temp_evening = 0.0
    street_temp_night = 0.0
    street_hum_morning = 0.0
    street_hum_day = 0.0
    street_hum_evening = 0.0
    street_hum_night = 0.0
    home_temp_morning = 0.0
    home_temp_day = 0.0
    home_temp_evening = 0.0
    home_temp_night = 0.0
    home_hum_morning = 0.0
    home_hum_day = 0.0
    home_hum_evening = 0.0
    home_hum_night = 0.0
    pressure_morning = 0.0
    pressure_day = 0.0
    pressure_evening = 0.0
    pressure_night = 0.0
    wind_speed_morning = 0.0
    wind_speed_day = 0.0
    wind_speed_evening = 0.0
    wind_speed_night = 0.0
    wind_direct_morning = ""
    wind_direct_day = ""
    wind_direct_evening = ""
    wind_direct_night = ""
    rain_morning = 0.0
    rain_day = 0.0
    rain_evening = 0.0
    rain_night = 0.0

    list_street_temp_morning = list()
    list_street_temp_day = list()
    list_street_temp_evening = list()
    list_street_temp_night = list()
    list_street_hum_morning = list()
    list_street_hum_day = list()
    list_street_hum_evening = list()
    list_street_hum_night = list()
    list_home_temp_morning = list()
    list_home_temp_day = list()
    list_home_temp_evening = list()
    list_home_temp_night = list()
    list_home_hum_morning = list()
    list_home_hum_day = list()
    list_home_hum_evening = list()
    list_home_hum_night = list()
    list_pressure_morning = list()
    list_pressure_day = list()
    list_pressure_evening = list()
    list_pressure_night = list()
    list_wind_speed_morning = list()
    list_wind_speed_day = list()
    list_wind_speed_evening = list()
    list_wind_speed_night = list()
    list_wind_direct_morning = list()
    list_wind_direct_day = list()
    list_wind_direct_evening = list()
    list_wind_direct_night = list()
    list_rain_morning = list()
    list_rain_day = list()
    list_rain_evening = list()
    list_rain_night = list()


    def make_report(self, start_time,end_time, editor_journal):
        logs = LogsWriter.Logs()
        logs.write_log("OK!    "+datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" Запуск python-скрипта для формирования pdf-отчета ")
        editor_journal.configure(state='normal')
        editor_journal.insert("insert", "OK    "+datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S")+"\t\t\tЗапуск python-скрипта для формирования pdf-отчета \n")
        editor_journal.configure(state='disabled')
        #print(datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" Запуск python-скрипта для формирования pdf-отчета ")
        MakeReport.find_now_params(self)
        MakeReport.find_params_each_time_of_day(self, start_time, end_time)
        pdf = FPDF()
        pdf.add_font("DejaVu", style="", fname="font/DejaVuSans.ttf", uni=True)
        pdf.add_font("DejaVu_bold", style="", fname="font/DejaVuSans-Bold.ttf", uni=True)
        pdf.add_font("Loma_bold", style="", fname="font/Loma-Bold.ttf", uni=True)
        pdf.add_page()
        pdf.set_font("Loma_bold", size=20)
        pdf.image("pictures/mainicon.jpg", x=90, y=10, w=40, h=40)
        for i in range(0,5):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.cell(200, 10, txt="-HomeWeatherStation-",ln=6, align='C')
        pdf.cell(200, 10, txt="Project", ln=7, align='C')
        pdf.set_font("Loma_bold", size=25)
        for i in range(8,10):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.set_font("DejaVu", size=25)
        pdf.cell(200, 10, txt="Отчет",ln=11, align='C')
        pdf.set_font("DejaVu", size=20)
        pdf.cell(200, 10, txt="с результатами измерений метеорологических", ln=12, align='C')
        pdf.cell(200, 10, txt="параметров за сутки", ln=13, align='C')
        pdf.set_font("DejaVu", size=16)
        for i in range(14,16):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.cell(200, 10, txt="Время начала ведения отчета:       "+start_time.strftime("%d/%b/%Y %Hч %Mмин %Sсек"), ln=17)
        pdf.cell(200, 10, txt="Время окончания ведения отчета: " + end_time.strftime("%d/%b/%Y %Hч %Mмин %Sсек"), ln=18)
        pdf.set_font("DejaVu", size=10)

        pdf.add_page()
        pdf.set_font("DejaVu_bold", size=20)
        pdf.cell(200, 10, txt="Погода СЕЙЧАС (на "+end_time.strftime("%d/%b/%Y %Hч %Mмин %Sсек")[12:]+")", ln=1, align='C')
        pdf.set_font("DejaVu_bold", size=17)
        pdf.cell(200, 10, txt="НА УЛИЦЕ:", ln=2, align='L')
        pdf.set_font("DejaVu", size=15)
        pdf.cell(200, 10, txt="                                     Температура:                     "+MakeReport.street_temp+" °C", ln=3, align='L')
        pdf.cell(200, 10, txt="                                     Влажность:                        "+MakeReport.street_hum+" %", ln=4, align='L')
        pdf.cell(200, 10, txt="                                     Кол-во осадков:                 "+MakeReport.rain+" %", ln=5, align='L')
        pdf.cell(200, 10, txt="                                     Скорость ветра:                 "+MakeReport.wind_speed+" м/с", ln=6, align='L')
        pdf.cell(200, 10, txt="                                     Направление ветра:           "+MakeReport.wind_direct, ln=7, align='L')
        for i in range(8,9):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.set_font("DejaVu_bold", size=17)
        pdf.cell(200, 10, txt="В ПОМЕЩЕНИИ:", ln=10, align='L')
        pdf.image("pictures/jpg_about_1.jpg", x=5, y=30, w=65, h=57)
        pdf.image("pictures/jpg_about_2.jpg", x=5, y=100, w=65, h=65)
        for i in range(11,12):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.set_font("DejaVu", size=15)
        pdf.cell(200, 10, txt="                                      Температура:                     "+MakeReport.home_temp+" °C", ln=13, align='L')
        pdf.cell(200, 10, txt="                                      Влажность:                        "+MakeReport.home_hum+" %", ln=14, align='L')
        pdf.cell(200, 10, txt="                                      Атмосферное давление:   "+MakeReport.pressure+" мм.рт.ст.", ln=15, align='L')
        pdf.set_font("DejaVu_bold", size=20)
        for i in range(16,19):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.cell(200, 10, txt="Погода в ТЕЧЕНИЕ СУТОК", ln=20, align='C')
        pdf.set_font("DejaVu_bold", size=17)
        pdf.cell(200, 10, txt="       Утро              День              Вечер              Ночь", ln=21, align='L')
        MakeReport.fill_picture(self, pdf)
        pdf.image("pictures/home_icon.png", x=5, y=220, w=10, h=10)
        pdf.image("pictures/street_icon.png", x=5, y=235, w=10, h=10)
        pdf.image("pictures/wind_icon.png", x=5, y=247, w=10, h=10)
        pdf.image("pictures/pressure_icon.png", x=5, y=259, w=10, h=10)
        pdf.image("pictures/rain_icon.png", x=5, y=272, w=10, h=10)
        pdf.set_font("DejaVu", size=14)
        for i in range(22,25):
            pdf.cell(200, 10, txt="", ln=i, align='C')
        pdf.cell(200, 5, txt="          %.1f °C                      %.1f °C                      %.1f °C                      %.1f °C" %(MakeReport.home_temp_morning, MakeReport.home_temp_day, MakeReport.home_temp_evening, MakeReport.home_temp_night), ln=26, align='L')
        pdf.cell(200, 5, txt="          %.1f %%                       %.1f %%                       %.1f %%                       %.1f %%" %(MakeReport.home_hum_morning, MakeReport.home_hum_day, MakeReport.home_hum_evening, MakeReport.home_hum_night), ln=27, align='L')
        pdf.cell(200, 5, txt="", ln=28, align='C')
        pdf.cell(200, 5,txt="          %.1f °C                      %.1f °C                      %.1f °C                      %.1f °C" %(MakeReport.street_temp_morning, MakeReport.street_temp_day, MakeReport.street_temp_evening, MakeReport.street_temp_night),ln=29, align='L')
        pdf.cell(200, 5,txt="          %.1f %%                       %.1f %%                       %.1f %%                       %.1f %%" %(MakeReport.street_hum_morning, MakeReport.street_hum_day, MakeReport.street_hum_evening, MakeReport.street_hum_night),ln=30, align='L')
        pdf.cell(200, 2, txt="", ln=31, align='C')
        pdf.cell(200, 5,txt="          %.1f м/с                     %.1f м/с                     %.1f м/с                     %.1f м/с" %(MakeReport.wind_speed_morning, MakeReport.wind_speed_day, MakeReport.wind_speed_evening, MakeReport.wind_speed_night),ln=32, align='L')
        pdf.cell(200, 5,txt="          %s                             %s                             %s                            %s" %(MakeReport.wind_direct_morning, MakeReport.wind_direct_day, MakeReport.wind_direct_evening, MakeReport.wind_direct_night),ln=33, align='L')
        pdf.cell(200, 2, txt="", ln=34, align='C')
        pdf.cell(200, 8,txt="          %d мм.рт.ст          %d мм.рт.ст         %d мм.рт.ст        %d мм.рт.ст" %(MakeReport.pressure_morning, MakeReport.pressure_day, MakeReport.pressure_evening, MakeReport.pressure_night),ln=35, align='L')
        pdf.set_auto_page_break(10)
        pdf.cell(200, 15,txt="          %.1f %%                       %.1f %%                       %.1f %%                       %.1f %%" %(MakeReport.rain_morning, MakeReport.rain_day, MakeReport.rain_evening, MakeReport.rain_night),ln=36, align='L')

        #file_report = open("debug/"+datetime.datetime.now().strftime("%d-%b-%Y %H:%M:%S") + ".txt","w")
        myconn = mysql.connector.connect(host=HOST, user=USER, passwd=PASSWORD, database=DATABASE)
        cur = myconn.cursor()
        pdf.add_page()
        pdf.set_font("DejaVu", size=15)
        pdf.cell(200, 10, txt="Журнал с подробными результатами всех измерений", ln=1, align='C')
        pdf.set_font("DejaVu_bold", size=10)
        pdf.cell(200, 5, txt="Дом.   Ул.   Дом.   Ул.   Давл.  Ск. Напр. Осадки   АКБ       Время приема      Время измерения", ln=3)
        pdf.cell(200, 5,txt="тем.  тем.  влаж.  влаж.       ветра ветра                              сервером ",ln=4)
        pdf.cell(200, 5, txt="[°C]   [°C]   [%]    [%] [мм.р.с] [м/с]            [%]      [В]", ln=5)
        pdf.set_font("DejaVu", size=10)
        try:
            cur.execute("select id, HomeTemp, StreetTemp, HomeHum, StreetHum, Pressuare, WindSpeed, WindDirect, Rain, BatteryCharge, reading_time, MeasureTime from HomeWeatherStationIvnya")
            result = cur.fetchall()
            for row in result:
                date = datetime.datetime.strptime(str(row[10]), '%Y-%m-%d %H:%M:%S')
                if ((date >= start_time) and (date <= end_time)):
                    pdf.cell(200, 15, txt="%s    %s    %s    %s    %s    %s       %s         %s         %s      %s     %s" % (row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]), ln=6)
                    #file_report.write("%d       %s           %s           %s          %s            %s           %s           %s         %s         %s              %s               %s\n" % (row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]))
                    #print("%d       %s           %s           %s          %s            %s           %s           %s         %s         %s              %s               %s\n" % (row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]))
            logs.write_log("OK!    "+ datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" pdf-отчет успешно сформирован и отправлен! ")
            editor_journal.configure(state='normal')
            editor_journal.insert("insert", "OK    "+ datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S")+"\t\t\tpdf-отчет успешно сформирован и отправлен! \n")
            editor_journal.configure(state='disabled')
            #print(datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" pdf-отчет успешно сформирован и отправлен! ")
        except:
            logs.write_log("ERROR! "+ datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" pdf-отчет не удалось сформировать и отправить! ОШИБКА! ")
            editor_journal.configure(state='normal')
            editor_journal.insert("insert", "ERROR "+ datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S")+"\t\t\tpdf-отчет не удалось сформировать и отправить! ОШИБКА! \n")
            editor_journal.configure(state='disabled')
            #print(datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" pdf-отчет не удалось сформировать и отправить! ОШИБКА! ")
            myconn.rollback()

        myconn.close()
        #file_report.close()
        file_name="Отчет_за_сутки_"+datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+".pdf"
        pdf.output("debug/"+"Отчет_за_сутки_"+datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+".pdf", 'F')
        return file_name

    def find_now_params(self):
        try:
            myconn = mysql.connector.connect(host=HOST, user=USER, passwd=PASSWORD, database=DATABASE)
            cur = myconn.cursor()
            cur.execute(
                "select id, HomeTemp, StreetTemp, HomeHum, StreetHum, Pressuare, WindSpeed, WindDirect, Rain, BatteryCharge, reading_time, MeasureTime from HomeWeatherStationIvnya")
            result = cur.fetchall()
            for row in result:
                MakeReport.street_temp = row[2]
                MakeReport.street_hum = row[4]
                MakeReport.home_temp = row[1]
                MakeReport.home_hum = row[3]
                MakeReport.pressure = row[5]
                MakeReport.wind_speed = row[6]
                MakeReport.wind_direct = row[7]
                MakeReport.rain = row[8]
            if (MakeReport.wind_direct == "N"):
                MakeReport.wind_direct = "Северный"
            elif (MakeReport.wind_direct == "S"):
                MakeReport.wind_direct = "Южный"
            elif (MakeReport.wind_direct == "W"):
                MakeReport.wind_direct = "Западный"
            elif (MakeReport.wind_direct == "E"):
                MakeReport.wind_direct = "Восточный"
            elif (MakeReport.wind_direct == "N-W"):
                MakeReport.wind_direct = "Северо-Западный"
            elif (MakeReport.wind_direct == "N-E"):
                MakeReport.wind_direct = "Северо-Восточный"
            elif (MakeReport.wind_direct == "S-E"):
                MakeReport.wind_direct = "Юго-Восточный"
            elif (MakeReport.wind_direct == "S-W"):
                MakeReport.wind_direct = "Юго-Западный"
        except:
            myconn.rollback()
        finally:
            myconn.close()

    def find_params_each_time_of_day(self, start_time, end_time):
        try:
            myconn = mysql.connector.connect(host=HOST, user=USER, passwd=PASSWORD, database=DATABASE)
            cur = myconn.cursor()
            cur.execute(
                "select id, HomeTemp, StreetTemp, HomeHum, StreetHum, Pressuare, WindSpeed, WindDirect, Rain, BatteryCharge, reading_time, MeasureTime from HomeWeatherStationIvnya")
            result = cur.fetchall()
            for row in result:
                MakeReport.cur_date = datetime.datetime.strptime(str(row[10]), '%Y-%m-%d %H:%M:%S')
                if((MakeReport.cur_date >= start_time) and (MakeReport.cur_date <= end_time)):
                    if ((MakeReport.cur_date.hour == 23) or (MakeReport.cur_date.hour < 5)):
                        if (row[2] != "NULL"): MakeReport.list_street_temp_night.append(row[2])
                        if (row[4] != "NULL"): MakeReport.list_street_hum_night.append(row[4])
                        MakeReport.list_home_temp_night.append(row[1])
                        MakeReport.list_home_hum_night.append(row[3])
                        MakeReport.list_pressure_night.append(row[5])
                        if (row[6] != "NULL"): MakeReport.list_wind_speed_night.append(row[6])
                        if (row[7] != "NULL"): MakeReport.list_wind_direct_night.append(row[7])
                        if (row[8] != "NULL"): MakeReport.list_rain_night.append(row[8])
                    elif((MakeReport.cur_date.hour >= 5) and (MakeReport.cur_date.hour < 11)):
                        if (row[2] != "NULL"): MakeReport.list_street_temp_morning.append(row[2])
                        if (row[4] != "NULL"): MakeReport.list_street_hum_morning.append(row[4])
                        MakeReport.list_home_temp_morning.append(row[1])
                        MakeReport.list_home_hum_morning.append(row[3])
                        MakeReport.list_pressure_morning.append(row[5])
                        if (row[6] != "NULL"): MakeReport.list_wind_speed_morning.append(row[6])
                        if (row[7] != "NULL"): MakeReport.list_wind_direct_morning.append(row[7])
                        if (row[8] != "NULL"): MakeReport.list_rain_morning.append(row[8])
                    elif ((MakeReport.cur_date.hour >= 11) and (MakeReport.cur_date.hour < 17)):
                        if (row[2] != "NULL"): MakeReport.list_street_temp_day.append(row[2])
                        if (row[4] != "NULL"): MakeReport.list_street_hum_day.append(row[4])
                        MakeReport.list_home_temp_day.append(row[1])
                        MakeReport.list_home_hum_day.append(row[3])
                        MakeReport.list_pressure_day.append(row[5])
                        if (row[6] != "NULL"): MakeReport.list_wind_speed_day.append(row[6])
                        if (row[7] != "NULL"): MakeReport.list_wind_direct_day.append(row[7])
                        if (row[8] != "NULL"): MakeReport.list_rain_day.append(row[8])
                    elif ((MakeReport.cur_date.hour >= 17) and (MakeReport.cur_date.hour < 23)):
                        if (row[2] != "NULL"): MakeReport.list_street_temp_evening.append(row[2])
                        if (row[4] != "NULL"): MakeReport.list_street_hum_evening.append(row[4])
                        MakeReport.list_home_temp_evening.append(row[1])
                        MakeReport.list_home_hum_evening.append(row[3])
                        MakeReport.list_pressure_evening.append(row[5])
                        if (row[6] != "NULL"):  MakeReport.list_wind_speed_evening.append(row[6])
                        if (row[7] != "NULL"): MakeReport.list_wind_direct_evening.append(row[7])
                        if (row[8] != "NULL"): MakeReport.list_rain_evening.append(row[8])
            MakeReport.street_temp_morning = MakeReport.find_medium(self, MakeReport.list_street_temp_morning)
            MakeReport.street_temp_day = MakeReport.find_medium(self, MakeReport.list_street_temp_day)
            MakeReport.street_temp_evening = MakeReport.find_medium(self, MakeReport.list_street_temp_evening)
            MakeReport.street_temp_night = MakeReport.find_medium(self, MakeReport.list_street_temp_night)
            MakeReport.street_hum_morning = MakeReport.find_medium(self, MakeReport.list_street_hum_morning)
            MakeReport.street_hum_day = MakeReport.find_medium(self, MakeReport.list_street_hum_day)
            MakeReport.street_hum_evening = MakeReport.find_medium(self, MakeReport.list_street_hum_evening)
            MakeReport.street_hum_night = MakeReport.find_medium(self, MakeReport.list_street_hum_night)
            MakeReport.home_temp_morning = MakeReport.find_medium(self, MakeReport.list_home_temp_morning)
            MakeReport.home_temp_day = MakeReport.find_medium(self, MakeReport.list_home_temp_day)
            MakeReport.home_temp_evening = MakeReport.find_medium(self, MakeReport.list_home_temp_evening)
            MakeReport.home_temp_night = MakeReport.find_medium(self, MakeReport.list_home_temp_night)
            MakeReport.home_hum_morning = MakeReport.find_medium(self, MakeReport.list_home_hum_morning)
            MakeReport.home_hum_day = MakeReport.find_medium(self, MakeReport.list_home_hum_day)
            MakeReport.home_hum_evening = MakeReport.find_medium(self, MakeReport.list_home_hum_evening)
            MakeReport.home_hum_night = MakeReport.find_medium(self, MakeReport.list_home_hum_night)
            MakeReport.pressure_morning = MakeReport.find_medium(self, MakeReport.list_pressure_morning)
            MakeReport.pressure_day = MakeReport.find_medium(self, MakeReport.list_pressure_day)
            MakeReport.pressure_evening = MakeReport.find_medium(self, MakeReport.list_pressure_evening)
            MakeReport.pressure_night = MakeReport.find_medium(self, MakeReport.list_pressure_night)
            MakeReport.wind_speed_morning = MakeReport.find_medium(self, MakeReport.list_wind_speed_morning)
            MakeReport.wind_speed_day = MakeReport.find_medium(self, MakeReport.list_wind_speed_day)
            MakeReport.wind_speed_evening = MakeReport.find_medium(self, MakeReport.list_wind_speed_evening)
            MakeReport.wind_speed_night = MakeReport.find_medium(self, MakeReport.list_wind_speed_night)
            MakeReport.wind_direct_morning = MakeReport.find_max_wind_direct(self, MakeReport.list_wind_direct_morning)
            MakeReport.wind_direct_day = MakeReport.find_max_wind_direct(self, MakeReport.list_wind_direct_day)
            MakeReport.wind_direct_evening = MakeReport.find_max_wind_direct(self, MakeReport.list_wind_direct_evening)
            MakeReport.wind_direct_night = MakeReport.find_max_wind_direct(self, MakeReport.list_wind_direct_night)
            MakeReport.rain_morning = MakeReport.find_medium(self, MakeReport.list_rain_morning)
            MakeReport.rain_day = MakeReport.find_medium(self, MakeReport.list_rain_day)
            MakeReport.rain_evening = MakeReport.find_medium(self, MakeReport.list_rain_evening)
            MakeReport.rain_night = MakeReport.find_medium(self, MakeReport.list_rain_night)
        except:
            myconn.rollback()
        finally:
            myconn.close()

    def find_medium(self, List):
        sum = 0.0
        medium = 0.0
        for value in List:
            sum += float(value)

        try:
            medium = sum/len(List)
        except:
            medium = 0.0
        return medium

    def find_max_wind_direct(self, List):
        N_counter = 0
        S_counter = 0
        E_counter = 0
        W_counter = 0
        NE_counter = 0
        NW_counter = 0
        SE_counter = 0
        SW_counter = 0
        max_counter = ""
        max = 0
        for value in List:
            if (value == "N"):
                N_counter += 1
            elif (value == "S"):
                S_counter += 1
            elif (value == "E"):
                E_counter += 1
            elif (value == "W"):
                W_counter += 1
            elif (value == "N-E"):
                NE_counter += 1
            elif (value == "N-W"):
                NW_counter += 1
            elif (value == "S-E"):
                SE_counter += 1
            elif (value == "S-W"):
                SW_counter += 1
        if (len(List) != 0):
            if (N_counter > S_counter):
                max_counter = "С"
                max = N_counter
            else:
                max_counter = "Ю"
                max = S_counter
            if (max > E_counter):
                max = max
            else:
                max_counter = "В"
                max = E_counter
            if (max > W_counter):
                max = max
            else:
                max_counter = "З"
                max = W_counter
            if (max > NE_counter):
                max = max
            else:
                max_counter = "С-В"
                max = NE_counter
            if (max > NW_counter):
                max = max
            else:
                max_counter = "С-З"
                max = NW_counter
            if (max > SE_counter):
                max = max
            else:
                max_counter = "Ю-В"
                max = SE_counter
            if (max > SW_counter):
                max = max
            else:
                max_counter = "Ю-З"
                max = SW_counter

        return max_counter

    def fill_picture(self, pdf):
        if ((MakeReport.wind_speed_morning == 0.0) and (MakeReport.rain_morning <= 10.0)):
            pdf.image("pictures/weath_sunny.jpg", x=15, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_morning >= 10.0) and (MakeReport.rain_morning <= 10.0)):
            pdf.image("pictures/weath_only_clouds.jpg", x=15, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_morning < 10.0) and (MakeReport.rain_morning <= 10.0)):
            pdf.image("pictures/weath_cloud.jpg", x=15, y=190, w=38, h=29)
        elif ((MakeReport.rain_morning < 90.0) and (MakeReport.rain_morning > 15.0)):
            pdf.image("pictures/weath_rainy.jpg", x=15, y=190, w=38, h=29)

        if ((MakeReport.wind_speed_day == 0.0) and (MakeReport.rain_day <= 10.0)):
            pdf.image("pictures/weath_sunny.jpg", x=60, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_day >= 10.0) and (MakeReport.rain_day <= 10.0)):
            pdf.image("pictures/weath_only_clouds.jpg", x=60, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_day < 10.0) and (MakeReport.rain_day <= 10.0)):
            pdf.image("pictures/weath_cloud.jpg", x=60, y=190, w=38, h=29)
        elif ((MakeReport.rain_day < 90.0) and (MakeReport.rain_day > 15.0)):
            pdf.image("pictures/weath_rainy.jpg", x=60, y=190, w=38, h=29)

        if ((MakeReport.wind_speed_evening == 0.0) and (MakeReport.rain_evening <= 10.0)):
            pdf.image("pictures/weath_sunny.jpg", x=110, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_evening >= 10.0) and (MakeReport.rain_evening <= 10.0)):
            pdf.image("pictures/weath_only_clouds.jpg", x=110, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_evening < 10.0) and (MakeReport.rain_evening <= 10.0)):
            pdf.image("pictures/weath_cloud.jpg", x=110, y=190, w=38, h=29)
        elif ((MakeReport.rain_evening < 90.0) and (MakeReport.rain_evening > 15.0)):
            pdf.image("pictures/weath_rainy.jpg", x=110, y=190, w=38, h=29)

        if ((MakeReport.wind_speed_night == 0.0) and (MakeReport.rain_night <= 10.0)):
            pdf.image("pictures/weath_sunny.jpg", x=160, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_night >= 10.0) and (MakeReport.rain_night <= 10.0)):
            pdf.image("pictures/weath_only_clouds.jpg", x=160, y=190, w=38, h=29)
        elif ((MakeReport.wind_speed_night < 10.0) and (MakeReport.rain_night <= 10.0)):
            pdf.image("pictures/weath_cloud.jpg", x=160, y=190, w=38, h=29)
        elif ((MakeReport.rain_night < 90.0) and (MakeReport.rain_night > 15.0)):
            pdf.image("pictures/weath_rainy.jpg", x=160, y=190, w=38, h=29)