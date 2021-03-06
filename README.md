# playrix
# Тестовое задание на позицию игровой программист: "Тир"

- Описание задачи
- Материалы и инструменты
- Как мы оцениваем результат
- Сроки и формат сдачи тестового задания

# Описание задачи

Задача: создать игру “Тир”.

Игра выглядит следующим образом: по экрану движутся N пузырей (мишеней) в

разных направлениях и с разной скоростью. При столкновении с краем экрана они

отскакивают. Внизу по центру экрана расположена пушка (или любой другой

стреляющий предмет). По клику из пушки вылетает снаряд и мчится к цели. Снаряд

летит достаточно быстро, а чтобы было интереснее, можно добавить к его свойствам

влияние силы тяжести или других факторов (например, отскоки снарядов от стен или

перегрев пушки). Если мишень сбили, то соответствующий объект должен быть удален

из памяти. Задача игрока — за отведенное время успеть сбить все мишени. После

окончания уровня должна быть возможность рестарта с удалением всех старых

объектов и созданием новых.

Алгоритм игры реализуется программно. Когда он будет реализован, в программу

нужно добавить использование партикловых эффектов — это позволит сделать игру

более красивой и зрелищной.

В игре нужно использовать два партикловых эффекта. С помощью первого эффекта

необходимо сделать шлейф, который будет следовать за снарядом. Второй эффект

должен визуально подчеркивать момент попадания снаряда в мишень.

Количество мишеней, изначально присутствующих на экране, скорость снаряда и

время, отведенное на игру, задаются в файле input.txt. По истечении времени игры

нужно вывести на экран результат: сколько целей из общего количества было

поражено.

Файл содержит 3 строки текста и выглядит следующим образом (константы вам

необходимо задать самостоятельно):

CountTarget=20

Speed=50

Time=50

Это означает, что на экране изначально присутствуют 20 мишеней, снаряд летит со

скоростью 50 пикселей в секунду, на игру отведено 50 секунд.

Пожалуйста, обратите внимание на то, что ваша основная задача —

запрограммировать алгоритм игры: полет мишеней, работу пушки, полет снаряда,

столкновение снаряда с мишенями. Программная реализация алгоритма этой игры

составляет основную часть тестового задания.

Помимо выполнения задания, ответьте, пожалуйста, на вопросы:

1. Примерную оценку времени, которое вы потратили на выполнение задания.

2. Что было делать интереснее — визуальную или техническую часть.

3. Комментарии по архитектуре приложения: почему выбрана именно такая

архитектура.

4. Возникали ли проблемы при выполнении тестового задания.

Данное тестовое, на наш взгляд, позволяет хорошо оценить как навыки

программирования эффектов, так и навыки реализации алгоритмов и построения

архитектуры приложения, что в дальнейшем позволит правильно определить спектр

рабочих задач. Условно можно разделить тестовое на 2 части: визуальная и

техническая.

Есть желание заниматься визуальными эффектами? В такой ситуации мы ожидаем

увидеть:

- красивые эффекты, речь здесь не только о партиклах, но и о программных

анимациях элементов игры, возможно, о применении шейдеров и т.д.,

- хорошо подобранные текстуры, выдержанные в одном стиле,

- подходящий фон, а его ведь еще можно и заанимировать :)

Мы никак не ограничиваем фантазию кандидатов.

Может быть, вам более близка техническая сторона вопроса? В таком случае хотелось

бы увидеть:

- грамотно построенную архитектуру, которая могла бы позволить

масштабировать и развивать приложение,

- оптимально реализованные алгоритмы,

- можно реализовать интересную механику выстрела или поведения целей,

возможно, какую-то физику.

Мы предполагаем, что вы не будете использовать физические движки, такие как Box2d,

например.

Стоит отметить, что мы не предлагаем выбрать одно из двух, можно сделать упор на

обе части.

# Материалы и инструменты

Тестовое задание выполняется на языке программирования С++. Вы можете

выполнить его на любом удобном для себя движке. Если выбор движка не имеет для

вас значения или у вас нет опыта работы с движками, то можно выполнить задание на

внутреннем движке нашей компании.

● Материалы для выполнения заданий на нашем движке: PC и Mac.

● C основной информацией о движке вы можете ознакомиться здесь.

● Описание редактора эффектов можно найти здесь.

Если вы решили использовать наш движок, то стартовый проект для Visual Studio 2015

и XCode можно найти в приложенных материалах. Он должен компилироваться и

запускаться без ошибок, но, возможно, нужно будет поправить working directory в

настройках проекта. В стартовом проекте есть шаблон решения с примером рисования

текстуры, привязанной к курсору мыши, простым партикловым эффектом, который

следует за мышью (с needStartDeadCounter = false) и еще с одним простым эффектом

(c needStartDeadCounter = true), который запускается по щелчку правой кнопкой мыши.

А также есть несколько примеров партикловых эффектов с готовым набором частиц.

Рекомендации по использованию нашего движка:

● Для реализации красивых и зрелищных эффектов можно использовать

партикловые эффекты, матричные преобразования, distortion и т.д.

● Партикловые эффекты создаются самостоятельно в редакторе эффектов.

Редактор с описанием и примерами эффектов прилагается к задаче. Пример

работы с эффектами есть в шаблоне решения.

● Для работы со сплайнами можно использовать готовый класс движка

TimedSpline. Пример работы с ним можно найти в документе Основы

использования движка и в шаблоне решения.

# Как мы оцениваем результат

При проверке тестового задания мы обращаем внимание на следующее:

● Отсутствие ошибок в реализации логики игры и алгоритмов.

● Правильное использование ООП и качество кода.

● Игровую составляющую (нужно настроить константы так, чтобы играть было

интересно).

● Визуальную составляющую.

# Сроки и формат сдачи тестового задания

Ссылку на выполненное тестовое задание вы можете прислать на почту рекрутеру,

выдавшему вам тестовое задание, с копией на общую почту отдела HR —

job@playrix.com.

По срокам выполнения тестового задания мы вас не ограничиваем — вы можете

прислать его, когда будете полностью в нем уверены. Как правило, кандидаты

справляются с ним за одну рабочую неделю.

При отправке тестового задания (через файлообменник, например, Dropbox),

необходимо приложить:

1. Запускаемое тестовое приложение и все необходимые для его запуска ресурсы.

2. Исходный код: достаточно прислать только h- и cpp-файлы (если вы выполняли

задание на нашем движке, не нужно отправлять папки "engine" и "libs").

3. Название движка в том случае, если задание выполнено на стороннем движке

(не следует высылать сам движок).

Пожалуйста, вышлите тестовое задание, включая приложение, ресурсы и ответы на

вопросы, единым архивом.

Перед тем, как приступить к выполнению тестового задания, пожалуйста, обратите

ваше внимание на то, что оно не является оплачиваемым. Также, предоставляя

результат вашего тестового задания, вы принимаете тот факт, что ООО “Плейрикс”

может в настоящее время и/или в будущем разрабатывать внутри компании или

получать от третьих лиц идеи и другие материалы, похожие по содержанию на

присланные вами, но ни в коем случае не основанные на них.

Мы еще раз благодарим вас за интерес к нашей компании и желаем удачи с

выполнением тестового задания! Очень надеемся, оно покажется вам интересным :)
