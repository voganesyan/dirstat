# dirstat
 collection of statistics about the different types of files contained in the directory
 
 English:
Usage statistics of the file system.
There is a tree of directories  of the selected disk. When you select a folder the statistics about it shows:
  - The number of files, total size, and average size for each file group (extension) - recursively(throughout subdirectories).
  - The number of subdirectories - only for that folder.
The user interface displays the status of all asynchronous operations - construction of a directory tree, counting statistics on the directory.

Counting statistics runs in a another thread, so the user GUI is not blocked.
Written in C ++ / Qt.
 
 Russian:
 Статистика использования файловой системы. 
Есть дерево каталогов выбранного диска. При выборе папки показывается статистика по ней:
 - количество файлов, общий размер и средний размер для каждой группы файлов (группировать по расширению и отдельной строкой - для всех) - рекурсивно по всей папке.
 - количество подкаталогов - только для данной папки. 
Интерфейс пользователя должен отображать статус всех асинхронных операций - построение дерева каталогов, подсчет статистки по каталогу. 

Подсчет статистики должен производиться в фоновом потоке, интерфейс пользователя не должен замирать или блокироваться.
Писать на С++/Qt, любых версий. Использование средств C++ 11 допускается.


