import subprocess
import http_request
import time, datetime
import telegram_bot
import threading, sys, os
import mysql.connector
import Email_bot
import LogsWriter
import report
from tkinter import *
from tkinter import messagebox
from threading import Thread
from tkinter import ttk
import sys
from tkinter.messagebox import showerror, showwarning, showinfo
from tkinter.scrolledtext import ScrolledText

TIME_UPDATE_DATA_BASE = 60 #время обновления публичной БД в сек
TIME_SEND_TG_AND_EMAIL = 1 #время отправки данных в телеграмм и Email в днях
TIME_CLEAN_DB = 360 #частота с которой проверяется БД на наличие актуальных данных за последнюю неделю и удаляет данные старше недели (в секундах)

#выбор режима подключения к ЛВС
CONNECTION_MODE = "ETHERNET"
#CONNECTION_MODE = "WIFI"


def check_wifi_connection():
    if CONNECTION_MODE == "WIFI":
        ps = subprocess.Popen(['iwgetid'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            output = subprocess.check_output(('grep', 'ESSID'), stdin=ps.stdout)
            #logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S ") + output)
            return True
        except subprocess.CalledProcessError:
            #logs.write_log("ERROR! "+ datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S")+" Отсутствует wi-fi соединение!")
            return False
    else:
        return True
def start_server():
    global Stop_Server
    Wi_Fi_Error_Flag = False
    #Старт работы с логами и очистка файла с логами
    logs=LogsWriter.Logs()
    logs.clean_log()
    logs.write_log("OK!    "+"Запуск работы сервера для проекта HomeWeatherStation...")
    editor_journal.configure(state='normal')
    editor_journal.insert("insert", "Статус" + "\tДата" +"\t\t\tСообщение\n")
    editor_journal.insert("insert", "OK    "+ datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") +"\t\t\tЗапуск работы сервера для проекта HomeWeatherStation...\n")
    editor_journal.configure(state='disabled')
    #Проверка wi-fi соединения
    if check_wifi_connection() != True:
        logs.write_log("ERROR! " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Отсутствует wi-fi соединение!")
        editor_journal.configure(state='normal')
        editor_journal.insert("insert", "ERROR " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tОтсутствует wi-fi соединение!\n")
        editor_journal.configure(state='disabled')
        os._exit(0)
    else:
        logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " wi-fi соединение успешно установлено!")
        editor_journal.configure(state='normal')
        editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\twi-fi соединение успешно установлено!\n")
        editor_journal.configure(state='disabled')
    #Проверка подключения к локальной сети (к маршрутизатору)
    response = os.system("ping -c 1 " + "192.168.1.1 > /home/alexey/HomeWeatherStationProject/debug/ping.txt")
    if response == 0:
        logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Подключение к ЛВС установлено!")
        editor_journal.configure(state='normal')
        editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tподключение к ЛВС установлено!\n")
        editor_journal.configure(state='disabled')
    else:
        logs.write_log("ERROR! " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Отсутствует подключение к ЛВС!")
        editor_journal.configure(state='normal')
        editor_journal.insert("insert", "ERROR " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tОтсутствует подключение к ЛВС!\n")
        editor_journal.configure(state='disabled')
        os._exit(0)
    #Запуск телеграмм-бота
    tg=telegram_bot.Telegramm()
    tg.start_Tg_BOT(editor_journal)

    http_req = http_request.HTTP_REQUEST()
    #Первичная очистка публичной БД
    http_req.clean_public_table(editor_journal)

    start_time_for_request=datetime.datetime.now()
    start_time_for_tg=datetime.datetime.now()
    start_time_for_clean_db=datetime.datetime.now()
    #Запуск Email-бота
    #email_bot = Email_bot.Email()
    #первичная проверка и удаление данных старше недели в локальной БД
    http_req.clean_table_for_weak(editor_journal)
    #Обновление публичной БД первый раз
    http_req.send_data_from_local_to_public_server(editor_journal,editor_table_BD)

    while(1):
        #Если выходим из программы
        if (Stop_Server == True):
            tg.send_close_info_tg(editor_journal)
            logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " РАБОТА СЕРВЕРА БЫЛА ОСТАНОВЛЕНА!")
            editor_journal.configure(state='normal')
            editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tРАБОТА СЕРВЕРА БЫЛА ОСТАНОВЛЕНА!\n")
            editor_journal.configure(state='disabled')
            Stop_Server = False
            os._exit(0)
            window.destroy()
            break
        #Проверка подключения к локальной сети
        response = os.system("ping -c 1 " + "192.168.1.1 > /home/alexey/HomeWeatherStationProject/debug/ping.txt")
        if response != 0:
            logs.write_log("ERROR! " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Отсутствует подключение к ЛВС! Перезагружаем WiFi адаптер...")
            editor_journal.configure(state='normal')
            editor_journal.insert("insert", "ERROR " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tОтсутствует подключение к ЛВС! Перезагружаем WiFi адаптер...\n")
            editor_journal.configure(state='disabled')
            while(os.system("ping -c 1 " + "192.168.1.1 > /home/alexey/HomeWeatherStationProject/debug/ping.txt")!=0):
                os.system("sudo ifconfig wlan0 down")
                time.sleep(10)
                os.system("sudo ifconfig wlan0 up")
                time.sleep(60)
            logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Подключение к ЛВС восстановлено!")
            editor_journal.configure(state='normal')
            editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tподключение к ЛВС восстановлено!\n")
            editor_journal.configure(state='disabled')

        diff_for_request=datetime.datetime.now()-start_time_for_request
        diff_for_tg = datetime.datetime.now() - start_time_for_tg
        diff_for_clean_db = datetime.datetime.now() - start_time_for_clean_db
        #Отправка сообщения в Tg, если не было Wi-Fi,а при этом должны были отправляться отчеты
        if (check_wifi_connection() != False) and (Wi_Fi_Error_Flag==True):
            time.sleep(60)
            tg.send_wifi_error_tg(editor_journal)
            Wi_Fi_Error_Flag = False
        #Обновление публичной БД каждые 60сек
        if (diff_for_request.seconds > TIME_UPDATE_DATA_BASE):
            if (check_wifi_connection() != False) and (Wi_Fi_Error_Flag!=True):
                http_req.send_data_from_local_to_public_server(editor_journal,editor_table_BD)
                Wi_Fi_Error_Flag = False
            else:
                logs.write_log("ERROR! " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Отсутствует wi-fi соединение!")
                editor_journal.configure(state='normal')
                editor_journal.insert("insert", "ERROR " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tОтсутствует wi-fi соединение!\n")
                editor_journal.configure(state='disabled')
                Wi_Fi_Error_Flag=True
            start_time_for_request=datetime.datetime.now()
        #Отправка отчета за сутки в Tg и Email каждые 24 часа
        if (diff_for_tg.days >= TIME_SEND_TG_AND_EMAIL):
            if (check_wifi_connection() != False) and (Wi_Fi_Error_Flag!=True):
                now_time=datetime.datetime.now()
                #Формириование отчета
                Report = report.MakeReport()
                time.sleep(5)
                pdf_report_file_name = Report.make_report(start_time_for_tg, now_time, editor_journal)
                #Отправка в Tg
                if tg.send_report_tg(pdf_report_file_name,start_time_for_tg, now_time, editor_journal) == False:
                     time.sleep(2)
                     logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + ' 2-я попытка отправки отчета в Tg-bot! ')
                     editor_journal.configure(state='normal')
                     editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + '\t\t\t2-я попытка отправки отчета в Tg-bot! \n')
                     editor_journal.configure(state='disabled')
                     time.sleep(2)
                     if tg.send_report_tg(pdf_report_file_name,start_time_for_tg, now_time, editor_journal) == False:
                         logs.write_log("OK!    " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + ' 3-я попытка отправки отчета в Tg-bot! ')
                         editor_journal.configure(state='normal')
                         editor_journal.insert("insert", "OK    " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + '\t\t\t3-я попытка отправки отчета в Tg-bot! \n')
                         editor_journal.configure(state='disabled')
                         time.sleep(2)
                         tg.send_report_tg(pdf_report_file_name, start_time_for_tg, now_time, editor_journal)
                         time.sleep(2)
                # Отправка на Email
                #email_bot.sendMail(pdf_report_file_name, editor_journal)
                #time.sleep(2)
                Wi_Fi_Error_Flag = False
                # Проверка локальной и публичной БД на необходимость очистки
                #if http_req.clean_local_table(editor_journal) == True:
                #    http_req.clean_public_table(editor_journal)
            else:
                logs.write_log("ERROR! " + datetime.datetime.now().strftime("%d-%b-%Y_%H:%M:%S") + " Отсутствует wi-fi соединение!")
                editor_journal.configure(state='normal')
                editor_journal.insert("insert", "ERROR " + datetime.datetime.now().strftime("\t%d-%b-%Y_%H:%M:%S") + "\t\t\tОтсутствует wi-fi соединение!\n")
                editor_journal.configure(state='disabled')
                Wi_Fi_Error_Flag = True
            start_time_for_tg = datetime.datetime.now()
        #Периодическая проверка того, что в БД находятся актуальные данные за последнюю неделю
        if (diff_for_clean_db.seconds >= TIME_CLEAN_DB):
            http_req.clean_table_for_weak(editor_journal)
            start_time_for_clean_db = datetime.datetime.now()
            #start_time_for_clean_db = start_time_for_clean_DB.day + 1

def clicked_btn_start():
    thread1.start()
    btn_start["state"] = DISABLED
    btn_stop["state"] = NORMAL
def clicked_btn_stop():
    global Stop_Server
    result = messagebox.askyesno(title="Предупреждение",message="Вы действительно хотите завершить работу сервера?")
    if result:
        showwarning(title="Предупреждение",message="Работа сервера будет завершена! Пожалуйста, подождите...")
        Stop_Server = True
        #exit()
    else:
        pass
    # thread1.join()
def on_close():
    global Stop_Server
    result = messagebox.askyesno(title="Предупреждение",message="Вы действительно хотите завершить работу сервера?")
    if result:
        showwarning(title="Предупреждение", message="Работа сервера будет завершена! Пожалуйста, подождите...")
        Stop_Server = True
        #exit()
    else:
        pass


Stop_Server = False
thread1 = Thread(target=start_server)

#Настройки окна
window = Tk()
window.title("Панель управления сервером для HomeWeatherStation")  # Добавляем заголовок окна
#window.geometry('900x600')
window.resizable(width=False, height=False)
w = window.winfo_screenwidth()
h = window.winfo_screenheight()
w = w//2 # середина экрана
h = h//2
w = w - 475 # смещение от середины
h = h - 300
window.geometry('950x600+{}+{}'.format(w, h))
# создаем набор вкладок
notebook = ttk.Notebook(width=950, height=620)
notebook.place(x=0, y=80)
# создаем фреймы
frame1 = ttk.Frame(notebook, width=110, height=30)
frame2 = ttk.Frame(notebook)
frame1.pack(fill=BOTH, expand=True)
frame2.pack(fill=BOTH, expand=True)
# добавляем фреймы в качестве вкладок
notebook.add(frame1, text="Журнал работы сервера")
notebook.add(frame2, text="Локальная База Данных")
#Кнопки
btn_start = Button(text="Запуск сервера!", command=clicked_btn_start, bg="green", fg="white")
btn_start.place(x=300, y=20)
btn_stop = Button(text="Остановка сервера!", command=clicked_btn_stop, bg="red", fg="white")
btn_stop.place(x=550, y=20)
btn_stop["state"] = DISABLED
#Окно с журналом
editor_journal = ScrolledText(frame1, width=115,  height=25, font = ("Times New Roman",12))
editor_journal.place(x=10, y=10)
editor_journal.configure(state ='disabled')
#Окно с таблицей измерений
editor_table_BD = ScrolledText(frame2, width=115,  height=25, font = ("Times New Roman",12))
editor_table_BD.place(x=10, y=10)
editor_table_BD.insert("insert", "№" + "\tДом." + "\tУл." + "\tДом." + "\tУл." + "\tДавле-" + "\tСкор-ть" + "\tНаправ." + "\tОсадки" + "\tЗаряд" + "\tВремя" + "\t\t     Время\n")
editor_table_BD.insert("insert", "\tтемп." + "\tтемп." + "\tвлажн." + "\tвлажн." + "\tние" + "\tветра" + "\tветра" + "\t\tАКБ" + "\tприема в БД" + "\t\t     измерения\n")
editor_table_BD.configure(state ='disabled')



window.protocol('WM_DELETE_WINDOW', on_close)

window.mainloop()  # Запускаем бесконечный цикл окна
