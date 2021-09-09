# DAQ Readme
Программа для системы сбора данных состоит из двух частей:
• daq - основная программа для передачи данных на компьютер online;
• daq_opt - дополнительная программа с оконным интерфейсом для
включения/выключения данных отправляющихся на запись.
Они работают независимо друг от друга. daq_opt записывает настройки в
settings.conf, а daq их вычитывает. Пути к settings.conf прописываются в
объекте settings созданным Qsettings в обоих программах.
Для сборки(редактирования) программ требуется установленный
фреймворк QT Creator. Дополнительно для сборки daq требуется библиотека
libssh-0.7.5. Её пути прописаны в daq.pro. Для работы daq требуется драйвер
QBUS: http://pcbech.ihep.su/bec/daq/qbus.html. В директории с программой daq
должен находится файл settings.xml, в котором задан host/port к online. 
Для существующей версии daq_opt для выбора в меню доступны
параметры записанные темным жирным шрифтом как на рис.1. Серые
параметры не доступны и задокументированы. При нажатие кнопки OK
происходит закрытие daq_opt и сохранение выставленных параметров. Для
закрытия без изменений нажимать на крестик. Вносить изменения следует
когда daq не запущен.
