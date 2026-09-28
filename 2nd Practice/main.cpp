#define NOMINMAX
#include <algorithm>
#include <array>
#include <climits>
#include <clocale>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

void setupRussianConsole() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_ALL, ".UTF-8");
#else
    std::setlocale(LC_ALL, "");
#endif
}

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void pauseAndClear() {
    std::cout << "\nНажмите Enter, чтобы продолжить...";
    std::string line;
    std::getline(std::cin, line);
    clearConsole();
}

template <typename T>
bool readNumber(T& value, const char* name) {
    
    std::string line;

    if (!std::getline(std::cin, line)) {

        std::cin.clear();
        std::cout << "Ошибка: не удалось прочитать " << name << ".\n";
        return false;
    }
    if (line.empty()) {

        std::cout << "Ошибка: величина " << name << " должна быть числом.\n";
        return false;
    }

    std::stringstream stream(line);
    T temp{};
    char extra{};

    if (!(stream >> temp) || (stream >> extra)) {

        std::cout << "Ошибка: величина " << name << " должна быть корректным числом.\n";
        return false;
    }
    value = temp;
    return true;
}

bool readIntInRange(int& value, const char* name, int minValue, int maxValue) {
    if (!readNumber(value, name)) {
        return false;
    }

    if (value < minValue || value > maxValue) {
        std::cout << "Ошибка: " << name 
            << " должно быть в диапазоне от " 
            << minValue << " до " << maxValue << ".\n";
        return false;
    }
    return true;
}

bool readNonNegativeInt(int& value, const char* name) {
    return readIntInRange(value, name, 0, INT_MAX);
}

bool readPositiveInt(int& value, const char* name) {

    return readIntInRange(value, name, 1, INT_MAX);
}

bool readRequiredString(std::string& value, const char* name, std::size_t maxLength = 100) {
    if (!std::getline(std::cin, value)) {

        std::cin.clear();
        std::cout << "Ошибка: не удалось прочитать " << name << ".\n";

        return false;
    }
    if (value.find_first_not_of(" \t\r\n") == std::string::npos) {

        std::cout << "Ошибка: " << name << " не может быть пустым.\n";

        return false;
    }
    if (value.size() > maxLength) {

        std::cout << "Ошибка: " << name << " не может содержать больше " << maxLength << " символов.\n";

        return false;
    }
    return true;
}

enum HeroClass { Warrior, Mage, Healer };

enum ItemType { Weapon, Armor, Potion };

enum StatusEffect { None, Blessed, Cursed, Poisoned };

struct StatusFlags {
    unsigned int blessed : 1;
    unsigned int cursed : 1;
    unsigned int poisoned : 1;
};

struct WeaponStats {
    int damage;
    int durability;
    bool twoHanded;
};

struct ArmorStats {
    int defense;
    int durability;
    bool magical;
};

struct PotionStats {
    int healAmount;
    int manaAmount;
};

union ItemProperties {
    WeaponStats weapon;
    ArmorStats armor;
    PotionStats potion;
};

struct Item {
    std::string name;
    ItemType type;
    int weight;
    int value;
    ItemProperties props;
};

struct Hero {
    std::string name;
    HeroClass heroClass;
    int level;
    int currentHP;
    int maxHp;
    int currentMP;
    int maxMp;
    StatusFlags status{};
    Item* equipment = nullptr;
    int equippedCount = 0;
};

struct Enemy {
    std::string name;
    int hp;
    int damage;
};

void printStatusFlags(const StatusFlags& status) {

    bool hasStatus = false;

    if (status.blessed) {

        std::cout << "Благословен";
        hasStatus = true;
    }

    if (status.cursed) {

        if (hasStatus) {
            std::cout << ", ";
        }

        std::cout << "Проклят";
        hasStatus = true;
    }

    if (status.poisoned) {

        if (hasStatus) {
            std::cout << ", ";
        }

        std::cout << "Отравлен";
        hasStatus = true;
    }

    if (!hasStatus) {
        std::cout << "[пусто]";
    }
}


int main() {

    setupRussianConsole();

    constexpr int InventorySize = 16;
    constexpr int EquipmentSize = 4;

    std::array<Item, InventorySize> inventory{};
    std::vector<Hero> heroes;

	Enemy* enemies = nullptr;
    int enemyCount = 0;

    // Оружие
    inventory[0].name = "Стальной меч";
    inventory[0].type = Weapon;
    inventory[0].weight = 5;
    inventory[0].value = 100;
    inventory[0].props.weapon = { 20, 50, false };

    inventory[1].name = "Боевой топор";
    inventory[1].type = Weapon;
    inventory[1].weight = 9;
    inventory[1].value = 150;
    inventory[1].props.weapon = { 30, 45, true };

    inventory[2].name = "Посох мистика";
    inventory[2].type = Weapon;
    inventory[2].weight = 3;
    inventory[2].value = 220;
    inventory[2].props.weapon = { 18, 35, false };

    inventory[3].name = "Серебряный кинжал";
    inventory[3].type = Weapon;
    inventory[3].weight = 2;
    inventory[3].value = 130;
    inventory[3].props.weapon = { 12, 70, false };

    // броня
    inventory[4].name = "Кольчуга";
    inventory[4].type = Armor;
    inventory[4].weight = 12;
    inventory[4].value = 180;
    inventory[4].props.armor = { 10, 60, false };

    inventory[5].name = "Башенный щит";
    inventory[5].type = Armor;
    inventory[5].weight = 10;
    inventory[5].value = 200;
    inventory[5].props.armor = { 18, 80, false };

    inventory[6].name = "Мантия архимага";
    inventory[6].type = Armor;
    inventory[6].weight = 4;
    inventory[6].value = 260;
    inventory[6].props.armor = { 6, 40, true };


    // зелья
    inventory[7].name = "Большое зелье";
    inventory[7].type = Potion;
    inventory[7].weight = 2;
    inventory[7].value = 70;
    inventory[7].props.potion = { 30, 20 };

    inventory[8].name = "Малое зелье здоровья";
    inventory[8].type = Potion;
    inventory[8].weight = 1;
    inventory[8].value = 30;
    inventory[8].props.potion = { 20, 0 };

    inventory[9].name = "Зелье маны";
    inventory[9].type = Potion;
    inventory[9].weight = 1;
    inventory[9].value = 50;
    inventory[9].props.potion = { 0, 50 };

    inventory[10].name = "Эликсир восстановления";
    inventory[10].type = Potion;
    inventory[10].weight = 2;
    inventory[10].value = 120;
    inventory[10].props.potion = { 35, 35 };

    // Артурчик
    Hero warrior{};

    warrior.name = "Артур";
    warrior.heroClass = Warrior;
    warrior.level = 2;

    warrior.maxHp = 100 + warrior.level * 20;
    warrior.currentHP = warrior.maxHp;

    warrior.maxMp = 10 + warrior.level * 2;
    warrior.currentMP = warrior.maxMp;

    warrior.status = { 1, 0, 0 };

    warrior.equipment = new Item[EquipmentSize]{};
    warrior.equippedCount = 2;

    warrior.equipment[0] = inventory[0];
    inventory[0] = Item{};

    warrior.equipment[1] = inventory[1];
    inventory[1] = Item{};

    heroes.push_back(warrior);

    // Мирочка
    Hero mage{};

    mage.name = "Мира";
    mage.heroClass = Mage;
    mage.level = 2;

    mage.maxHp = 50 + mage.level * 8;
    mage.currentHP = mage.maxHp;

    mage.maxMp = 100 + mage.level * 25;
    mage.currentMP = mage.maxMp;

    mage.status = { 0, 0, 1 };

    mage.equipment = new Item[EquipmentSize]{};
    mage.equippedCount = 2;

    mage.equipment[0] = inventory[4];
    inventory[4] = Item{};

    mage.equipment[3] = inventory[2];
    inventory[2] = Item{};

    heroes.push_back(mage);

    // Элианочка
    Hero healer{};

    healer.name = "Элиана";
    healer.heroClass = Healer;
    healer.level = 2;

    healer.maxHp = 60 + healer.level * 10;
    healer.currentHP = healer.maxHp;

    healer.maxMp = 80 + healer.level * 20;
    healer.currentMP = healer.maxMp;

    healer.status = { 0, 0, 0 };

    healer.equipment = new Item[EquipmentSize]{};
    healer.equippedCount = 2;

    healer.equipment[0] = inventory[3];
    inventory[3] = Item{};

    healer.equipment[3] = inventory[7];
    inventory[7] = Item{};

    heroes.push_back(healer);

    // хтонь
    enemyCount = 4;
    enemies = new Enemy[enemyCount];

    enemies[0] = { "Гоблин", 100, 15 };
    enemies[1] = { "Орк с 4chan", 150, 20 };
	enemies[2] = { "Дракон", 300, 40 };
	enemies[3] = { "Тролль", 200, 25 };

    bool running = true;

    while (running) {
        std::cout << "\n『 ГЛАВНОЕ МЕНЮ 』\n"
            << "1. Управление отрядом\n" 
            << "2. Управление инвентарём\n" 
            << "3. Экипировка героев\n"
            << "4. Управление врагами\n" 
            << "5. Симуляция битвы\n"
            << "6. Сводка по отряду\n" 
            << "0. Выход\n" 
            << "Ваш выбор: ";

        int mainChoice = 0;

        if (!readIntInRange(mainChoice, "пункт меню", 0, 6)) {
            pauseAndClear(); 
            continue; 
        }
        else if (mainChoice == 0) {
            running = false; 
            continue; 
        }

        clearConsole();

        switch (mainChoice) {
        
		case 1: {

            bool back = false;

            while (!back) {
                std::cout << "\n『 УПРАВЛЕНИЕ ОТРЯДОМ 』\n"
                    << "1. Добавить героя\n" 
                    << "2. Удалить героя\n" 
                    << "3. Показать всех героев\n" 
                    << "0. Назад\n" 
                    << " Ваш выбор: ";

                int choice = 0;

                if (!readIntInRange(choice, "пункт меню", 0, 3)) { 
                    pauseAndClear(); 
                    continue; 
                }

                if (choice == 1) {

                    Hero hero{};

                    while (true) {
                        std::cout << "Введите имя героя: ";

                        if (readRequiredString(hero.name, "имя героя")) {
                            break;
                        }

                        std::cout << "Попробуйте ещё раз.\n";
                    }

                    int classChoice = 0;

                    while (true) {
                        std::cout << "Выберите класс"
                            << " (1 - боец, 2 - маг, 3 - целитель): ";

                        if (readIntInRange(classChoice, "класс", 1, 3)) {
                            break;
                        }

                        std::cout << "Попробуйте ещё раз.\n";
                    }

                    hero.heroClass =
                        static_cast<HeroClass>(classChoice - 1);

                    long long maxHp = 0;
                    long long maxMp = 0;

                    while (true) {
                        std::cout << "Введите уровень героя: ";

                        if (!readPositiveInt(hero.level, "уровень")) {
                            std::cout << "Попробуйте ещё раз.\n";
                            continue;
                        }

                        if (hero.heroClass == Warrior) {
                            maxHp = 100LL + hero.level * 20LL;
                            maxMp = 10LL + hero.level * 2LL;
                        }
                        else if (hero.heroClass == Mage) {
                            maxHp = 50LL + hero.level * 8LL;
                            maxMp = 100LL + hero.level * 25LL;
                        }
                        else {
                            maxHp = 60LL + hero.level * 10LL;
                            maxMp = 80LL + hero.level * 20LL;
                        }

                        if (maxHp > INT_MAX || maxMp > INT_MAX) {
                            std::cout
                                << "Ошибка: такой уровень слишком большой "
                                << "для хранения характеристик героя.\n"
                                << "Введите меньший уровень.\n";

                            continue;
                        }

                        break;
                    }

                    hero.maxHp = static_cast<int>(maxHp);
                    hero.maxMp = static_cast<int>(maxMp);

                    hero.currentHP = hero.maxHp;
                    hero.currentMP = hero.maxMp;

                    int statusChoice = 0;

                    while (true) {
                        std::cout << "Выберите особенность"
                            << " (0 - отсутствует, 1 - благословлен,"
                            << " 2 - проклят, 3 - отравлен): ";

                        if (readIntInRange(statusChoice, "статус", 0, 3)) {
                            break;
                        }

                        std::cout << "Попробуйте ещё раз.\n";
                    }

                    hero.status = { 0, 0, 0 };

                    StatusEffect selectedEffect = static_cast<StatusEffect>(statusChoice);

                    switch (selectedEffect) {

                    case None:
                        break;

                    case Blessed:
                        hero.status.blessed = 1;
                        break;

                    case Cursed:
                        hero.status.cursed = 1;
                        break;

                    case Poisoned:
                        hero.status.poisoned = 1;
                        break;
                    }

                    hero.equipment = new Item[EquipmentSize]{};
                    hero.equippedCount = 0;

                    heroes.push_back(hero);

                    std::cout << "Герой добавлен. Индекс: "
                        << heroes.size() - 1
                        << "\n";
                }
				else if (choice == 2) {
                    
                    if (heroes.empty()) { 
                        std::cout << "Отряд пуст.\n"; 
                        break; 
                    }

                    int index = 0;

                    std::cout << "Введите индекс героя для удаления: ";

                    if (!readIntInRange(index, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) 
                        break;

                    delete[] heroes[index].equipment;

                    if (index != static_cast<int>(heroes.size()) - 1) 
                        std::swap(heroes[index], heroes.back());

                    heroes.pop_back();

                    std::cout << "Герой удалён.\n";
                }
                else if (choice == 3) { ///просмотр отряда

                    if (heroes.empty()) { 
                        std::cout << "Отряд пуст.\n"; 
                        break; 
                    }

                    for (size_t i = 0; i < heroes.size(); ++i) {
                        
                        std::cout << "[" << i << "] " << heroes[i].name << " | ";

                        if (heroes[i].heroClass == Warrior) 
                            std::cout << "Warrior";
                        else if (heroes[i].heroClass == Mage) 
                            std::cout << "Mage";
						else if (heroes[i].heroClass == Healer)
                            std::cout << "Healer";

                        std::cout << " | Ур." << heroes[i].level 
                            << " | HP: " << heroes[i].currentHP << "/" << heroes[i].maxHp
                            << " | Мана: " << heroes[i].currentMP << "/" << heroes[i].maxMp 
                            << " | Статусы: ";
                        printStatusFlags(heroes[i].status);
                        std::cout << "\n";
                    }
                }
                else { 
                    back = true;
                }

                if (choice != 0 && !back)
                    pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 2: {

            bool back = false;

            while (!back) {
                std::cout << "\n『 УПРАВЛЕНИЕ ИНВЕНТАРЁМ 』\n"
                    << "1. Добавить предмет\n" 
                    << "2. Удалить предмет\n" 
                    << "3. Показать инвентарь\n"
                    << "0. Назад\n" 
                    << "Ваш выбор: ";

                int choice = 0;
                if (!readIntInRange(choice, "пункт меню", 0, 3)) { 
                    pauseAndClear(); 
                    continue; 
                }
                if (choice == 1) {

                    int freeIndex = -1;

                    for (int i = 0; i < InventorySize; ++i) 
                        if (inventory[i].name.empty()) { 
                            freeIndex = i; 
                            break; 
                        }

                    if (freeIndex == -1) { 
                        std::cout << "Инвентарь заполнен.\n";
                        break;
                    }

                    Item item{};

                    std::cout << "Введите название предмета: ";
                    if (!readRequiredString(item.name, "название предмета")) 
                        break;

                    int typeChoice = 0;

                    std::cout << "Выберите тип" 
                        << " (1 - Weapon, 2 - Armor, 3 - Potion): ";

                    if (!readIntInRange(typeChoice, "тип предмета", 1, 3)) 
                        break;

                    item.type = static_cast<ItemType>(typeChoice - 1);

                    std::cout << "Введите вес: "; if (!readNonNegativeInt(item.weight, "вес")) break;
                    std::cout << "Введите стоимость: "; if (!readNonNegativeInt(item.value, "стоимость")) break;

                    if (item.type == Weapon) {

                        std::cout << "Введите урон: "; 
                        if (!readNonNegativeInt(item.props.weapon.damage, "урон")) 
                            break;

                        std::cout << "Введите прочность: "; 
                        if (!readNonNegativeInt(item.props.weapon.durability, "прочность")) 
                            break;

                        int th = 0; 
                        std::cout << "Двуручное? (0 - нет, 1 - да): ";
                        if (!readIntInRange(th, "двуручность", 0, 1)) 
                            break;

                        item.props.weapon.twoHanded = (th == 1);
                    }
                    else if (item.type == Armor) {
                        std::cout << "Введите защиту: "; 
                        if (!readNonNegativeInt(item.props.armor.defense, "защита")) 
                            break;

                        std::cout << "Введите прочность: "; 
                        if (!readNonNegativeInt(item.props.armor.durability, "прочность")) 
                            break;

                        int mg = 0; 
                        std::cout << "Магическое? (0 - нет, 1 - да): ";
                        if (!readIntInRange(mg, "зачарованность", 0, 1))
                            break;
                        item.props.armor.magical = (mg == 1);
                    }
                    else {

                        std::cout << "Введите лечение здоровья: "; 
                        if (!readNonNegativeInt(item.props.potion.healAmount, "лечение")) 
                            break;

                        std::cout << "Введите восстановление маны: "; 
                        if (!readNonNegativeInt(item.props.potion.manaAmount, "MP")) 
                            break;
                    }

                    inventory[freeIndex] = item;

                    std::cout << "Предмет добавлен в слот " << freeIndex << ".\n";
                }
                else if (choice == 2) {

                    int index = 0;

                    std::cout << "Введите индекс слота для удаления: ";

                    if (!readIntInRange(index, "индекс слота", 0, InventorySize - 1)) 
                        break;

                    if (inventory[index].name.empty()) 
                        std::cout << "Этот слот уже пуст.\n";

                    else { inventory[index] = Item{}; 
                    std::cout << "Предмет удалён.\n"; 

                    }
                }
                else if (choice == 3) {

                    std::cout << "\n『 ИНВЕНТАРЬ 』\n";

                    for (int i = 0; i < InventorySize; ++i) {

                        if (inventory[i].name.empty()) {
                            std::cout << "[" << i << "] пусто\n";
                            continue;
                        }

                        std::cout << "\n[" << i << "] "
                            << "Название: " << inventory[i].name << "\n";

                        std::cout << "    Тип: ";

                        if (inventory[i].type == Weapon) {
                            std::cout << "Weapon\n";
                        }
                        else if (inventory[i].type == Armor) {
                            std::cout << "Armor\n";
                        }
                        else {
                            std::cout << "Potion\n";
                        }

                        std::cout << "    Вес: " << inventory[i].weight << "\n";
                        std::cout << "    Стоимость: " << inventory[i].value << "\n";

                        switch (inventory[i].type) {

                        case Weapon:
                            std::cout << "    Урон: "
                                << inventory[i].props.weapon.damage << "\n";
                            std::cout << "    Прочность: "
                                << inventory[i].props.weapon.durability << "\n";
                            std::cout << "    Двуручное: "
                                << (inventory[i].props.weapon.twoHanded ? "да" : "нет") << "\n";
                            break;

                        case Armor:
                            std::cout << "    Защита: "
                                << inventory[i].props.armor.defense << "\n";
                            std::cout << "    Прочность: "
                                << inventory[i].props.armor.durability << "\n";
                            std::cout << "    Магическое: "
                                << (inventory[i].props.armor.magical ? "да" : "нет") << "\n";
                            break;

                        case Potion:
                            std::cout << "    Лечение HP: "
                                << inventory[i].props.potion.healAmount << "\n";
                            std::cout << "    Восстановление MP: "
                                << inventory[i].props.potion.manaAmount << "\n";
                            break;
                        }
                    }
                }
                else { 
                    back = true; 
                }
                if (choice != 0 && !back) 
                    pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 3: {

            bool back = false;

            while (!back) {
                std::cout << "\n『 ЭКИПИРОВКА ГЕРОЕВ 』\n"
                    << "1. Надеть предмет\n" 
                    << "2. Снять предмет\n"
                    << "3. Показать экипировку\n"
                    << "0. Назад\n"
                    << "Ваш выбор: ";

                int choice = 0;

                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {
                    
                    if (heroes.empty()) { 
                        std::cout << "Нет героев.\n"; 
                        break; 
                    }

                    int hIdx = 0; std::cout << "Индекс героя: ";
                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) 
                        break;

                    int iIdx = 0; std::cout << "Индекс предмета: ";
                    if (!readIntInRange(iIdx, "индекс предмета", 0, InventorySize - 1)) 
                        break;

                    if (inventory[iIdx].name.empty()) { 
                        std::cout << "Слот инвентаря пуст.\n"; 
                        break; 
                    }

                    int freeSlot = -1;

                    for (int i = 0; i < EquipmentSize; ++i) {

                        if (heroes[hIdx].equipment[i].name.empty()) {
                            freeSlot = i;
                            break;
                        }
                    }

                    if (freeSlot == -1) std::cout << "Все 4 слота заняты.\n";

                    else {
                        heroes[hIdx].equipment[freeSlot] = inventory[iIdx];
                        inventory[iIdx] = Item{};
                        heroes[hIdx].equippedCount++;
                        
                        std::cout << "Предмет экипирован в слот " << freeSlot << ".\n";
                    }
                }
                else if (choice == 2) {
                    
                    if (heroes.empty()) { 
                        std::cout << "Нет героев.\n";
                        break; 
                    }

                    int hIdx = 0; std::cout << "Индекс героя: ";

                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1))
                        break;
                    int eIdx = 0; std::cout << "Слот экипировки (0-3): ";
                    
                    if (!readIntInRange(eIdx, "слот", 0, EquipmentSize - 1))
                        break;

                    if (heroes[hIdx].equipment[eIdx].name.empty()) { 
                        std::cout << "Слот пуст.\n";
                        break; 
                    }

                    int freeInv = -1;

                    for (int i = 0; i < InventorySize; ++i) {
                        
                        if (inventory[i].name.empty()) { 
                            freeInv = i; 
                            break; 
                        }
                    }

                    if (freeInv == -1) std::cout << "В инвентаре нет места.\n";
                    
                    else {
                        inventory[freeInv] = heroes[hIdx].equipment[eIdx];
                        heroes[hIdx].equipment[eIdx] = Item{};
                        heroes[hIdx].equippedCount--;
                        std::cout << "Предмет снят в слот " << freeInv << ".\n";
                    }
                }
                else if (choice == 3) {

                    if (heroes.empty()) {
                        std::cout << "Нет героев.\n";
                        break; 
                    }
                    
                    int hIdx = 0; std::cout << "Индекс героя: ";
                    
                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1))
                        break;
                    
                    std::cout << "\nЭкипировка " << heroes[hIdx].name << ":\n";
                    
                    for (int i = 0; i < EquipmentSize; ++i) {
                        
                        if (heroes[hIdx].equipment[i].name.empty())
                            std::cout << "[" << i << "] пусто\n";
                        else 
                            std::cout << "[" << i << "] " << heroes[hIdx].equipment[i].name << "\n";
                    }
                }
                else { 
                    back = true; 
                }

                if (choice != 0 && !back) pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 4: {
            bool back = false;

            while (!back) {
                
                std::cout << "\n『 УПРАВЛЕНИЕ ВРАГАМИ 』\n"
                    << "1. Добавить врага\n" 
                    << "2. Удалить врага\n" 
                    << "3. Показать врагов\n" 
                    << "0. Назад\n" 
                    << "Ваш выбор: ";

                int choice = 0;

                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {

                    char nameBuffer[100]{};
                    
                    std::cout << "Введите имя врага: ";

                    std::cin.getline(nameBuffer, sizeof(nameBuffer));
                    std::string eName(nameBuffer);

                    if (eName.find_first_not_of(" \t\r\n") == std::string::npos) { 
                        std::cout << "Имя не может быть пустым.\n"; 
                        break;
                    }

                    int hp = 0, dmg = 0;

                    std::cout << "Введите количество здоровья: "; 
                    if (!readPositiveInt(hp, "здоровье"))
                        break;

                    std::cout << "Введите урон: ";
                    if (!readPositiveInt(dmg, "урон"))
                        break;

                    Enemy* newEnemies = new Enemy[enemyCount + 1];

                    for (int i = 0; i < enemyCount; ++i) {

                        newEnemies[i] = enemies[i];
                    }

                    newEnemies[enemyCount] = { eName, hp, dmg };
                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount++;

                    std::cout << "Враг добавлен. Индекс: " << enemyCount - 1 << "\n";
                }
                else if (choice == 2) {
                    
                    if (enemyCount == 0) { 
                        std::cout << "Список пуст.\n";
                        break; 
                    }

                    int idx = 0; std::cout << "Индекс врага: ";

                    if (!readIntInRange(idx, "индекс", 0, enemyCount - 1)) 
                        break;

                    Enemy* newEnemies = nullptr;

                    if (enemyCount > 1) {

                        newEnemies = new Enemy[enemyCount - 1];

                        int nIdx = 0;

                        for (int i = 0; i < enemyCount; ++i)
                        {
                            if (i != idx) newEnemies[nIdx++] = enemies[i];
                        }
                    }

                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount--;

                    std::cout << "Враг удалён.\n";
                }
                else if (choice == 3) {

                    if (enemyCount == 0) { 
                        std::cout << "Список пуст.\n";
                        break; 
                    }

                    for (int i = 0; i < enemyCount; ++i) {
                        std::cout << "[" << i << "] " << enemies[i].name 
                            << " | здоровье: " << enemies[i].hp 
                            << " | урон: " << enemies[i].damage << "\n";
                    }
                }
                else { 
                    back = true; 
                }

                if (choice != 0 && !back) 
                    pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 5: {

            if (heroes.empty() || enemyCount == 0) { 

                std::cout << "Нужен хотя бы 1 герой и 1 враг.\n"; 
                pauseAndClear(); 
                break; 
            }

            int turn = 1;

            std::cout << "\n『 ГЕРОИ 』\n";

            for (size_t i = 0; i < heroes.size(); ++i) {

                std::cout << "[" << i << "] "
                    << heroes[i].name
                    << " | ";

                if (heroes[i].heroClass == Warrior) {
                    std::cout << "Warrior";
                }
                else if (heroes[i].heroClass == Mage) {
                    std::cout << "Mage";
                }
                else if (heroes[i].heroClass == Healer) {
                    std::cout << "Healer";
                }

                std::cout
                    << " | Ур. " << heroes[i].level
                    << " | HP: "
                    << heroes[i].currentHP
                    << "/" << heroes[i].maxHp
                    << " | MP: "
                    << heroes[i].currentMP
                    << "/" << heroes[i].maxMp
                    << " | Статусы: ";

                printStatusFlags(heroes[i].status);

                std::cout << "\n";
            }

            std::cout << "\n『 ВРАГИ 』\n";

            for (int i = 0; i < enemyCount; ++i) {

                std::cout
                    << "[" << i << "] "
                    << enemies[i].name
                    << " | HP: "
                    << enemies[i].hp
                    << " | Урон: "
                    << enemies[i].damage
                    << "\n";
            }

            std::cout << "РЕЗНЯЯЯЯ!\n";

            while (true) {

                bool anyHeroAlive = false;
                bool anyEnemyAlive = false;

                for (const auto& h : heroes) {

                    if (h.currentHP > 0) {
                        anyHeroAlive = true;
                    }
                }

                for (int i = 0; i < enemyCount; ++i) {

                    if (enemies[i].hp > 0) {
                        anyEnemyAlive = true;
                    }
                }

                if (!anyHeroAlive || !anyEnemyAlive)
                    break;

                std::cout << "\n『 ХОД " << turn << " 』\n";

                for (auto& h : heroes) {
                    if (h.currentHP <= 0) 
                        continue;

                    if (h.status.poisoned) {

                        h.currentHP -= 3;

                        if (h.currentHP < 0) h.currentHP = 0;

                        std::cout << h.name << " получает 3 урона от яда." 
                            << "HP: " << h.currentHP << "\n";
                    }
                }

                for (size_t hIdx = 0; hIdx < heroes.size(); ++hIdx) {

                    Hero& hero = heroes[hIdx];

                    if (hero.currentHP <= 0) continue;

                    anyEnemyAlive = false;

                    for (int i = 0; i < enemyCount; ++i) {

                        if (enemies[i].hp > 0) {
                            anyEnemyAlive = true;
                        }
                    }

                    if (!anyEnemyAlive) 
                        break;

                    std::cout << "\nХод героя: " << hero.name << "\n";

                    if (!hero.equipment[3].name.empty() && hero.equipment[3].type == Potion) {

                        int drink = 0;

                        std::cout << "Вам доступно зелье. Выпить? (1 - да, 0 - нет): ";

                        if (readIntInRange(drink, "выбор", 0, 1) && drink == 1) {

                            hero.currentHP += hero.equipment[3].props.potion.healAmount;
                            hero.currentMP += hero.equipment[3].props.potion.manaAmount;

                            if (hero.currentHP > hero.maxHp) 
                                hero.currentHP = hero.maxHp;

                            if (hero.currentMP > hero.maxMp) 
                                hero.currentMP = hero.maxMp;

                            hero.equipment[3] = Item{};
                            hero.equippedCount--;

                            std::cout << "Зелье использовано.\n";
                        }
                        else {
							std::cout << "Зелье не использовано.\n";
                        }
                    }

                    int abilityCost = (hero.heroClass == Warrior) ? 20 : 40;

                    bool canAbility = false;

                    if (hero.currentMP >= abilityCost) {

                        int useAbility = 0;

                        std::cout << "Использовать способность? (1 - да, 0 - нет): ";

                        if (!readIntInRange(useAbility, "способность", 0, 1))
                            continue;

                        if (useAbility == 1) {
                            canAbility = true;
                        }
                    }
                    else {
                        std::cout << "Недостаточно маны. Выполняется обычная атака.\n";
                    }

                    int weaponDmg = 0;

                    for (int i = 0; i < EquipmentSize; ++i) {

                        if (!hero.equipment[i].name.empty() && hero.equipment[i].type == Weapon) {
                            weaponDmg += hero.equipment[i].props.weapon.damage;
                        }
                    }

                    if (hero.heroClass == Healer && canAbility) {

                        int allyIdx = 0;

                        std::cout << "Индекс союзника для лечения: ";

                        if (readIntInRange(allyIdx, "союзник", 0, static_cast<int>(heroes.size()) - 1)) {
                            
                            if (heroes[allyIdx].currentHP > 0) {

                                hero.currentMP -= abilityCost;
                                int heal = 6 + weaponDmg;
                                heroes[allyIdx].currentHP += heal;

                                if (heroes[allyIdx].currentHP > heroes[allyIdx].maxHp) {
                                    heroes[allyIdx].currentHP = heroes[allyIdx].maxHp;
                                }

                                std::cout << hero.name << " лечит " << heroes[allyIdx].name << " на " << heal << " HP.\n";
                            }
                            else { std::cout << "Союзник мёртв.\n"; }
                        }
                    }
                    else {
                        int targetIdx = 0;

                        std::cout << "Индекс врага: ";
                        if (!readIntInRange(targetIdx, "враг", 0, enemyCount - 1)) 
                            continue;

                        if (enemies[targetIdx].hp <= 0) { std::cout << "Враг мёртв.\n"; continue; }

                        if (canAbility) hero.currentMP -= abilityCost;

                        double dmg = 0;
                        
                        if (hero.heroClass == Warrior)
                        {
                            dmg = canAbility ? (15.0 * 1.5) : (15.0 + weaponDmg);
                        }
                        else if (hero.heroClass == Mage) {
                            dmg = canAbility ? (8.0 * 3.0) : (8.0 + weaponDmg);
                        }
                        else {
                            dmg = 6.0 + weaponDmg;
                        }

                        if (hero.status.blessed)
                        {
                            dmg *= 2.0;
                        }
                        if (hero.status.cursed)
                        {
                            dmg = std::ceil(dmg / 2.0);
                        }

                        int finalDmg = static_cast<int>(dmg);

                        if (finalDmg < 1)
                        {
                            finalDmg = 1;
                        }

                        enemies[targetIdx].hp -= finalDmg;

                        if (enemies[targetIdx].hp < 0)
                        {
                            enemies[targetIdx].hp = 0;
                        }

                        std::cout << hero.name << " наносит " << finalDmg << " урона."
                            << " HP врага : " << enemies[targetIdx].hp << "\n";
                    }
                }

                anyEnemyAlive = false;

                for (int i = 0; i < enemyCount; ++i) {

                    if (enemies[i].hp > 0) {
                        anyEnemyAlive = true;
                    }
                }

                if (anyEnemyAlive) {

                    for (int i = 0; i < enemyCount; ++i) {

                        if (enemies[i].hp <= 0) 
                            continue;

                        int targetIdx = 0;

                        std::cout << "\nВраг " << enemies[i].name << " атакует. Индекс героя: ";

                        if (!readIntInRange(targetIdx, "герой", 0, static_cast<int>(heroes.size()) - 1)) 
                            continue;

                        if (heroes[targetIdx].currentHP <= 0) { 
                            std::cout << "Герой мёртв.\n"; 
                            continue; 
                        }

                        int armorDef = 0;

                        for (int slot = 0; slot < EquipmentSize; ++slot)
                        {
                            if (!heroes[targetIdx].equipment[slot].name.empty() && heroes[targetIdx].equipment[slot].type == Armor) {
                                armorDef += heroes[targetIdx].equipment[slot].props.armor.defense;
                            }
                        }

                        int dmg = enemies[i].damage - armorDef;

                        if (dmg < 1) {
                            dmg = 1;
                        }

                        heroes[targetIdx].currentHP -= dmg;

                        if (heroes[targetIdx].currentHP < 0) {
                            heroes[targetIdx].currentHP = 0;
                        }

                        std::cout << enemies[i].name << " наносит " << dmg << " урона." 
                            << " HP героя : " << heroes[targetIdx].currentHP << "\n";
                    }
                }

                std::cout << "\n『 СОСТОЯНИЕ ПОСЛЕ ХОДА 』\n";

                std::cout << "\nГерои:\n";

                for (size_t i = 0; i < heroes.size(); ++i) {

                    std::cout
                        << "[" << i << "] "
                        << heroes[i].name
                        << " | "
                        << (heroes[i].currentHP > 0 ? "жив" : "мёртв")
                        << " | HP: "
                        << heroes[i].currentHP
                        << "/" << heroes[i].maxHp
                        << " | MP: "
                        << heroes[i].currentMP
                        << "/" << heroes[i].maxMp
                        << " | Статусы: ";

                    printStatusFlags(heroes[i].status);

                    std::cout << "\n";
                }

                std::cout << "\nВраги:\n";

                for (int i = 0; i < enemyCount; ++i) {

                    std::cout
                        << "[" << i << "] "
                        << enemies[i].name
                        << " | "
                        << (enemies[i].hp > 0 ? "жив" : "мёртв")
                        << " | HP: "
                        << enemies[i].hp
                        << "\n";
                }

                ++turn;
            }

            bool anyHeroAlive = false;
            bool anyEnemyAlive = false;

            for (const auto& h : heroes) {

                if (h.currentHP > 0) {
                    anyHeroAlive = true;
                }
            }

            for (int i = 0; i < enemyCount; ++i) {
                if (enemies[i].hp > 0) {
                    anyEnemyAlive = true;
                }
            }

            std::cout << "\n『 РЕЗУЛЬТАТ БИТВЫ 』\n";

            if (anyHeroAlive && !anyEnemyAlive) {
                std::cout << "Победа героев!\n";
            }
            else if (!anyHeroAlive && anyEnemyAlive) {
                std::cout << "Победа врагов.\n";
            }
            else {
                std::cout << "Ничья.\n";
            }

            pauseAndClear();
            break;
        }
        case 6: {

            std::cout << "\n『 СВОДКА ПО ОТРЯДУ 』\n";

            long long totalLevel = 0; 
            long long totalHp = 0;
            long long totalMp = 0;

            for (const auto& h : heroes) { 

                totalLevel += h.level; 
                totalHp += h.currentHP;
                totalMp += h.currentMP;
            }

            std::cout << "Героев: " << heroes.size() << " | "
                << " Сумм. уровень: " << totalLevel << " | " 
                << " Сумм. HP: " << totalHp << " | " 
                << "Сумм. MP: " << totalMp << "\n\n";

            for (size_t i = 0; i < heroes.size(); ++i) {

                std::cout << "[" << i << "] " << heroes[i].name 
                    << " | HP " << heroes[i].currentHP << "/" << heroes[i].maxHp
                    << " | MP " << heroes[i].currentMP << "/" << heroes[i].maxMp << " | статусы: ";
                printStatusFlags(heroes[i].status);

                std::cout << "\n  Экипировка: ";
                bool hasEq = false;

                for (int slot = 0; slot < EquipmentSize; ++slot) {

                    if (!heroes[i].equipment[slot].name.empty()) {

                        if (hasEq) {
                            std::cout << ", ";
                        }

                        std::cout << heroes[i].equipment[slot].name;

                        hasEq = true;
                    }
                }
                if (!hasEq) {
                    std::cout << "нет";
                }

                std::cout << "\n";
            }

            int usedSlots = 0; 
            long long totalValue = 0;
            long long totalWeight = 0;

            for (const auto& item : inventory) {
                if (!item.name.empty()) {
                    ++usedSlots;
                    totalValue += item.value;
                    totalWeight += item.weight;
                }
            }

            std::cout << "\nИнвентарь: Занято " << usedSlots << "/16 | " 
                << " Стоимость: " << totalValue << " | " 
                << " Вес: " << totalWeight << "\n";

            std::cout << "\nВраги:\n";

            if (enemyCount == 0) {
                std::cout << "Нет врагов.\n";
            }
            else {

                for (int i = 0; i < enemyCount; ++i) {

                    std::cout << "[" << i << "] " << enemies[i].name
                        << " | HP: " << enemies[i].hp
                        << " | урон: " << enemies[i].damage
                        << "\n";
                }
            }

            pauseAndClear();
            break;
        }
        }
    }

    for (auto& hero : heroes) {
        delete[] hero.equipment;
        hero.equipment = nullptr;
    }

    heroes.clear();
    delete[] enemies;
    enemies = nullptr;

    clearConsole();
    std::cout << "Симуляция завершена. Брысь из матрицы.\n";
    return 0;
}