# FORK
Системный вызов fork() - "развилка". Создает новый процесс, процесс - потомок, 
который является копией процесса - предка, в том смысле, что процесс потомок наследует код предка, 
дескрипторы открытых файлов, маску создания файлов, сигнальную маску

В старых системах Unix код предка копировался в адрессное пространство потомка, 
т.е для потомка создавалось собественно адрессное пространство(виртуальное), 
в которое копировался код предка. в результате в системе одновременно могло 
существовать несколько копий одной и той же программы, что является не оптимальным, 
поэтому в современных системах используется так называемый оптимизированный  fork.

Проблема: 
- В старых системах код предка копировался

Решение - флаг copy on write

Он также создает копию процесса предка, точно так же создается собственно адресное пространство, 
но код предка не копируется в это адресное пространство, а дескрипторы страниц 
потомка ссылаются на СТРАНИЦЫ адресного пространства предка. 

Для страниц адресного пространства предка обычные права доступа RW 
меняются на only read и устанавливается флаг COPY ON WRITE (копирование при записи). 
Если или предок или потомок попытаются изменить какую-либо страницу, возникнет исключение 
по правам доступа.Выполняя это исключение система обнаружит установленный флаг 
COPY ON WRITE и создаст копию данной страницы в адрессном пространстве того процесса, 
который пытался ее изменить.В результате создаются только копии нужных страниц.

Такая ситуация с правами доступа и флагом COPY ON WRITE  будет существовать в системе до тех пор, пока потомок не вызовет системный вызов exit()  или системный вызов exec()

exit() - завершает процесс.
exec() - переводит процесс потомок на адресное пространство программы, которая передана exec в качестве параметра, в результате, потомок начинается выполнять другую программу. Другими словами, в Unix  для того, чтобы запустить программу сначала надо вызвать fork, а затем процесс потомок должен вызвать exec.

# EXEC
Системный вызов exec: для чего введён, какие действия выполняет, прежде чем будет запущена новая программа

Системный вызов exec переводит процесс на новое адресное пространство программы, которая передана ему в качестве параметра.
Как он выполняется:
1) проверяет, существует ли файл с именем, которое ему передали;
2) проверяет, исполняемый ли он (~для этого надо прочитать заголовок файла~);
3) проверяет, есть ли у процесса, который выполняет системный вызов exec, права на его запуск;
4) создаётся новое адресное пространство, т.е. создаются таблицы страниц для программы, переданной exec в качестве параметра;
5) удалить старые таблицы страниц, указатель на которые содержится в дескрипторе процесса;
6) поменять указатель на новые таблицы страниц;
7) загрузить в регистр IP начальный адрес программы (~или адрес точки входа программы, или просто точку входа программы~), а в регистр CR3 загрузить адрес таблиц страниц из дескриптора процесса (в дескрипторе мы поменяли адрес таблиц страниц, но его ещё надо загрузить в CR3), чтобы можно было выполнять страничное преобразование, которое нужно для адресации кода.

Загрузка CR3 нужна только в процессорах Intel.

Результат: будет выполняться новая программа.
Если exec был вызван в потомке, то у предка флаг copy on write будет сброшен и права доступа вернутся к read-write.