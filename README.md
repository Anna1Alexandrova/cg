lab 1.1
В этом задании для визуализации разрешается использовать произвольные графическое API и UI-framework. Допускается использовать и веб-технологии (HTML, CSS, JavaScript).
Разработайте GUI-приложение, отображающее при помощи векторных примитивов (прямоугольники, многоугольники и другие) Ваши инициалы разноцветными буквами.
Пример визуализации инициалов студента Петра Ильича Чайковского:
![image](https://github.com/user-attachments/assets/c9055f2f-64ae-4d74-ac96-7b3e2bb14159)
lab 1.2
В этом задании для визуализации разрешается использовать произвольное графическое API. Разрешается использовать и веб-технологии (HTML, CSS, JavaScript).
Разработайте GUI-приложение графическое приложение, визуализирующее с использованием векторных примитивов картинку на одну из следующих тем:
•	Домик в деревне. Домик с крышей, трубой, окном, дверью и забором.
•	Троллейбус. Троллейбус с несколькими окнами, колесами, а также токоприемниками, соединенными с проводами.
•	Один из героев мультсериала South Park. Постараться соблюсти визуальное сходство с прототипом
•	Системный блок компьютера и монитор (вид спереди). У системного блока должен быть виден CD-ROM, кнопка включения и цветные индикаторы (питание, жесткий диск).
•	Цветок. У цветка должно быть несколько листьев и лепестков, а также стебель.
•	Грузовик (вид сбоку). Кузов, колеса, окна, дверь.
•	Космическая ракета. 
•	Тема по выбору студента. Тема, предложенная студентом не должна уступать по сложности предложенным выше вариантам.

lab 2
Разработайте приложение для просмотра растровых изображений. При помощи меню File -> Open должен открыться стандартный диалог выбора файла. 
Должна поддерживаться возможность открытия хотя бы одного из стандартных типов файлов изображений: PNG, JPEG, BMP. 
Для файлов формата PNG-файлов должна поддерживаться полупрозрачность (под изображением нарисуйте просвечивающую сквозь него подложку, например, в виде шахматной доски). 
 ![image](https://github.com/user-attachments/assets/64d95fd4-2369-46b4-a797-29c42db222c2)

При изменении размеров окна изображение должно центрироваться в клиентской области окна. 
За пределы окна изображение выходить не должно – при нехватке места оно должно ресайзиться с сохранением оригинальных пропорций.


lab 3
Разработайте клон игры Тетрис, в котором в стакан шириной 10 и высотой 20 клеток падают случайные разноцветные фигурки, состоящие из четырех квадратов (тетрамино). При заполнении одной или нескольких линий стакана они удаляются, что приводит к падению вышестоящих линий, а игроку начисляются очки:
•	За одну заполненную линию – 10 очков
•	За две заполненные линии – 30 очков
•	За три заполненные линии – 70 очков
•	За четыре заполненные линии – 150 очков
Если падение очередной фигуры приведет к заполнению стакана, игра заканчивается и игроку предлагается начать игру заново, либо выйти из игры.
Пользователь может поворачивать падающие фигурки вокруг их центра на 90 градусов по часовой стрелке при помощи клавиши «курсор вверх», а также ускорять их падение при помощи клавиши «курсор вниз». Клавиши «курсор влево» и «курсор вправо» позволяют перемещать фигуру влево или вправо.
При помощи клавиши “P” игрок может поставить игру на паузу.
Информационная панель в игре отображает номер текущего уровня, количество линий, которое нужно заполнить для перехода на следующий уровень, набранное количество баллов и тип следующей фигуры.
Целью игры является заполнение заданного количества линий для прохождения на следующий уровень. При этом начисляется количество очков, равное количеству линий оставшихся полностью свободными от блоков, умноженное на 10. При переходе на новый уровень стакан очищается и увеличивается как скорость падения фигур, так и количество линий, которое нужно заполнить для его прохождения.

lab 4
Разработайте OpenGL-приложение, визуализирующее правильный многогранник, полуправильный многогранник, либо звездчатый многогранник, соответствующий одному из заданных вариантов. Смежные грани многогранника должны быть окрашены в разные цвета. Ребра многогранника должны иметь черный цвет.
