# EngineForGames

Небольшой движок для разработки 2D-игр и прототипов, написанный на <b>C++</b> с использованием современных библиотек.

## Технологии и библиотеки

<ul>
  <li><a href="https://ru.wikipedia.org/wiki/OpenGL" target="_blank">OpenGL</a> — для рендеринга графики</li>
  <li><a href="https://github.com/skypjack/entt">entt</a> — ECS-система для эффективного управления объектами</li>
  <li><a href="https://github.com/ocornut/imgui">ImGui</a> — интегрированный UI для отладки и редактирования</li>
  <li><a href="https://github.com/nlohmann/json">nlohmann/json</a> — для работы с конфигурациями и данными</li>
  <li><a href="https://github.com/nothings/stb">stb</a> — легковесные утилиты (например, загрузка текстур)</li>
</ul>

## Основные возможности

<h3>Управление объектами</h3>
<ul>
  <li>Изменение позиции (x, y)</li>
  <li>Настройка масштаба объекта</li>
  <li>Смещение текстуры (UV-сдвиг)</li>
  <li>Управление размером текстуры</li>
  <li>Задание центра вращения (origin point)</li>
  <li>Поворот объекта</li>
  <li>Изменение цвета текстуры (RGBA)</li>
  <li>Отражение по осям X и Y</li>
  <li>Оптимизация через <a href="https://en.wikipedia.org/wiki/Space_partitioning" target="_blank">Spatial Partitioning</a> — эффективный поиск объектов по координатам (рис. 1)</li>
</ul>

<div align="center">
  <img src="https://github.com/user-attachments/assets/6bd29aa1-7970-4528-b586-230cafcd9366" alt="Spatial Partitioning в действии" width="500">
  <p><em>Рисунок 1 — Spatial Partitioning</em></p>
</div>

<h3>Скриптинг</h3>

<ul>
  <li>Возможность прикреплять скрипты к любому объекту</li>
  <li>Каждый экземпляр объекта выполняет свой скрипт независимо</li>
  <li>Поддержка пользовательской логики через функции обновления</li>
  <li>Простая интеграция без глобального состояния</li>
</ul>


## Как выглядит:

https://github.com/user-attachments/assets/f183dc18-252e-4a6a-8e48-a39a30143767

<h3>Отладочная информация</h3>

Встроенная панель Debug Info отображает (рис. 2):

<ul>
  <li>Текущий FPS и время кадра</li>
  <li>Версия OpenGL и драйвера</li>
  <li>Размер окна и события изменения</li>
  <li>Ввод с клавиатуры и мыши</li>
  <li>Ввод символов (char input)</li>
  <li>Время последнего события</li>
</ul>

<div align="center">
  <img src="https://github.com/user-attachments/assets/12a521a4-eff9-4a45-ad9c-0677f22cbc58" alt="Панель Debug Info" width="500">
  <p><em>Рисунок 2 — Панель Debug Info</em></p>
</div>

## Планы на будущее

<ul>
  <li>Поддержка анимаций</li>
  <li>Интеграция физического движка</li>
  <li>Добавление звука</li>
  <li>Поддержка пользовательских шейдеров</li>
  <li>Экспорт и импорт сцены</li>
  <li>Переделать систему <a href="https://en.wikipedia.org/wiki/Space_partitioning" target="_blank">SpatialPartitioning</a></li>
</ul>

## Автор

<a href="https://github.com/sksi1217" target="_blank">GitHub профиль автора</a>


## Как запустить

Соберите проект с помощью CMake и запустите исполняемый файл:
```
cmake . -DBUILD_DEBUG=ON
```
```
make -j$(nproc) && cd build && cd .. && ./build/myapp
```



---


# Что можно сделать в движке

## Работа со скриптами и взаимодействие между объектами

Движок поддерживает гибкую систему скриптов на основе ECS (Entity Component System). Вы можете не только добавлять пользовательские скрипты к игровым объектам, но и устанавливать связи между объектами — например, чтобы один объект управлял другим. 

**Пример: переключение активности другого объекта**

**1. Определение скрипта** 

Внутри вашего скрипта вы можете хранить ссылку на другой объект через его `entt::entity` (уникальный идентификатор сущности). Это безопасно и соответствует принципам ECS: 
```cpp
class Script : public ScriptComponent
{
private:
public:
	entt::entity targetEntity = entt::null;

	void Update() override
	{
		if (InputManager::Get().WasKeyPressed(GLFW_KEY_SPACE))
		{
			if (targetEntity != entt::null && registry)
			{
				Object target(*registry, targetEntity);
				if (target.IsActive())
				{
					target.SetActive(false);
				}
				else
				{
					target.SetActive(true);
				}
			}
		}
	}
};

```
## 2. Настройка объектов в коде
При создании объектов вы можете связать их, передав entity одного объекта в скрипт другого:
```cpp
Object entity = Object::CreateObject(registry);
auto &transform = entity.GetComponent<Transform>();
transform.Position = {100, 100};
transform.Scale = {100, 100};
entity.AddComponent<Sprite>(texture.get());
entity.AddScript<ExampleScript>();
entity.AddScript<Script>();

Object entity1 = Object::CreateObject(registry);
auto &transform1 = entity1.GetComponent<Transform>();
transform1.Position = {100, 100};
transform1.Scale = {100, 100};
entity1.AddComponent<Sprite>(texture.get());

entity.GetScript<Script>().targetEntity = entity1.entity;
```
Теперь при нажатии Пробела первый объект будет включать/выключать второй.

Эта система позволяет легко строить сложные взаимодействия между игровыми сущностями, сохраняя при этом чистоту архитектуры и безопасность памяти.

---

