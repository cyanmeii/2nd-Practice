#define NOMINMAX
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

// ============================================================
// Вспомогательные функции
// ============================================================
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
    if (!readNumber(value, name)) return false;
    if (value < minValue || value > maxValue) {
        std::cout << "Ошибка: " << name << " должно быть в диапазоне от " << minValue << " до " << maxValue << ".\n";
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

// ============================================================
// Типы данных
// ============================================================
enum HeroClass { Warrior, Mage, Healer };
enum ItemType { Weapon, Armor, Potion };
enum StatusEffect { None, Blessed, Cursed, Poisoned };

struct StatusFlags {
    unsigned int blessed : 1;
    unsigned int cursed : 1;
    unsigned int poisoned : 1;
};

struct WeaponStats { int damage; int durability; bool twoHanded; };
struct ArmorStats { int defense; int durability; bool magical; };
struct PotionStats { int healAmount; int manaAmount; };

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
    int hp;
    int maxHp;
    int mp;
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
    if (status.blessed) { std::cout << "Blessed "; hasStatus = true; }
    if (status.cursed) { std::cout << "Cursed "; hasStatus = true; }
    if (status.poisoned) { std::cout << "Poisoned "; hasStatus = true; }
    if (!hasStatus) { std::cout << "None"; }
}

// ============================================================
// Главная функция
// ============================================================
int main() {
    setupRussianConsole();
    constexpr int InventorySize = 16;
    constexpr int EquipmentSize = 4;

    std::array<Item, InventorySize> inventory{};
    std::vector<Hero> heroes;
    Enemy* enemies = nullptr;
    int enemyCount = 0;

    // Демонстрационные данные
    inventory[0] = { "Стальной меч", Weapon, 5, 100, {20, 50, false} };
    inventory[1] = { "Кольчуга", Armor, 12, 180, {10, 60, false} };
    inventory[2] = { "Большое зелье", Potion, 2, 70, {30, 20} };

    Hero warrior{};
    warrior.name = "Артур";
    warrior.heroClass = Warrior;
    warrior.level = 2;
    warrior.maxHp = 140; warrior.hp = 140;
    warrior.maxMp = 14; warrior.mp = 14;
    warrior.status = { 1, 0, 0 };
    warrior.equipment = new Item[EquipmentSize]{};
    warrior.equippedCount = 2;
    warrior.equipment[0] = inventory[0];
    warrior.equipment[1] = inventory[1];
    inventory[0].name = ""; inventory[1].name = "";
    heroes.push_back(warrior);

    Hero mage{};
    mage.name = "Мира";
    mage.heroClass = Mage;
    mage.level = 2;
    mage.maxHp = 66; mage.hp = 66;
    mage.maxMp = 150; mage.mp = 150;
    mage.status = { 0, 0, 1 };
    mage.equipment = new Item[EquipmentSize]{};
    mage.equippedCount = 0;
    heroes.push_back(mage);

    enemyCount = 2;
    enemies = new Enemy[enemyCount];
    enemies[0] = { "Гоблин", 100, 15 };
    enemies[1] = { "Орк", 150, 20 };

    bool running = true;
    while (running) {
        std::cout << "\n========== ГЛАВНОЕ МЕНЮ ==========\n";
        std::cout << "1. Управление отрядом\n2. Управление инвентарём\n3. Экипировка героев\n";
        std::cout << "4. Управление врагами\n5. Симуляция битвы\n6. Сводка по отряду\n0. Выход\n";
        std::cout << "Ваш выбор: ";

        int mainChoice = 0;
        if (!readIntInRange(mainChoice, "пункт меню", 0, 6)) { pauseAndClear(); continue; }
        if (mainChoice == 0) { running = false; continue; }
        clearConsole();

        switch (mainChoice) {
        case 1: { // Управление отрядом
            bool back = false;
            while (!back) {
                std::cout << "\n----- УПРАВЛЕНИЕ ОТРЯДОМ -----\n";
                std::cout << "1. Добавить героя\n2. Удалить героя\n3. Показать всех героев\n0. Назад\nВаш выбор: ";
                int choice = 0;
                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {
                    Hero hero{};
                    std::cout << "Введите имя героя: ";
                    if (!readRequiredString(hero.name, "имя героя")) break;

                    int classChoice = 0;
                    std::cout << "Выберите класс (1 - Warrior, 2 - Mage, 3 - Healer): ";
                    if (!readIntInRange(classChoice, "класс", 1, 3)) break;
                    hero.heroClass = static_cast<HeroClass>(classChoice - 1);

                    std::cout << "Введите уровень героя: ";
                    if (!readPositiveInt(hero.level, "уровень")) break;

                    long long maxHp = 0, maxMp = 0;
                    if (hero.heroClass == Warrior) { maxHp = 100LL + hero.level * 20LL; maxMp = 10LL + hero.level * 2LL; }
                    else if (hero.heroClass == Mage) { maxHp = 50LL + hero.level * 8LL; maxMp = 100LL + hero.level * 25LL; }
                    else { maxHp = 60LL + hero.level * 10LL; maxMp = 80LL + hero.level * 20LL; }

                    hero.maxHp = static_cast<int>(maxHp); hero.maxMp = static_cast<int>(maxMp);
                    hero.hp = hero.maxHp; hero.mp = hero.maxMp;

                    int statusChoice = 0;
                    std::cout << "Выберите статус (0 - None, 1 - Blessed, 2 - Cursed, 3 - Poisoned): ";
                    if (!readIntInRange(statusChoice, "статус", 0, 3)) break;
                    hero.status = { 0, 0, 0 };
                    if (statusChoice == 1) hero.status.blessed = 1;
                    else if (statusChoice == 2) hero.status.cursed = 1;
                    else if (statusChoice == 3) hero.status.poisoned = 1;

                    hero.equipment = new Item[EquipmentSize]{};
                    heroes.push_back(hero);
                    std::cout << "Герой добавлен. Индекс: " << heroes.size() - 1 << "\n";
                }
                else if (choice == 2) {
                    if (heroes.empty()) { std::cout << "Отряд пуст.\n"; break; }
                    int index = 0;
                    std::cout << "Введите индекс героя для удаления: ";
                    if (!readIntInRange(index, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) break;
                    delete[] heroes[index].equipment;
                    if (index != static_cast<int>(heroes.size()) - 1) std::swap(heroes[index], heroes.back());
                    heroes.pop_back();
                    std::cout << "Герой удалён.\n";
                }
                else if (choice == 3) {
                    if (heroes.empty()) { std::cout << "Отряд пуст.\n"; break; }
                    for (size_t i = 0; i < heroes.size(); ++i) {
                        std::cout << "[" << i << "] " << heroes[i].name << " | ";
                        if (heroes[i].heroClass == Warrior) std::cout << "Warrior";
                        else if (heroes[i].heroClass == Mage) std::cout << "Mage";
                        else std::cout << "Healer";
                        std::cout << " | Lv." << heroes[i].level << " | HP: " << heroes[i].hp << "/" << heroes[i].maxHp
                            << " | MP: " << heroes[i].mp << "/" << heroes[i].maxMp << " | Статусы: ";
                        printStatusFlags(heroes[i].status);
                        std::cout << "\n";
                    }
                }
                else { back = true; }
                if (choice != 0 && !back) pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 2: { // Инвентарь
            bool back = false;
            while (!back) {
                std::cout << "\n----- УПРАВЛЕНИЕ ИНВЕНТАРЁМ -----\n";
                std::cout << "1. Добавить предмет\n2. Удалить предмет\n3. Показать инвентарь\n0. Назад\nВаш выбор: ";
                int choice = 0;
                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {
                    int freeIndex = -1;
                    for (int i = 0; i < InventorySize; ++i) if (inventory[i].name.empty()) { freeIndex = i; break; }
                    if (freeIndex == -1) { std::cout << "Инвентарь заполнен.\n"; break; }

                    Item item{};
                    std::cout << "Введите название предмета: ";
                    if (!readRequiredString(item.name, "название предмета")) break;

                    int typeChoice = 0;
                    std::cout << "Выберите тип (1 - Weapon, 2 - Armor, 3 - Potion): ";
                    if (!readIntInRange(typeChoice, "тип предмета", 1, 3)) break;
                    item.type = static_cast<ItemType>(typeChoice - 1);

                    std::cout << "Введите вес: "; if (!readNonNegativeInt(item.weight, "вес")) break;
                    std::cout << "Введите стоимость: "; if (!readNonNegativeInt(item.value, "стоимость")) break;

                    if (item.type == Weapon) {
                        std::cout << "Введите урон: "; if (!readNonNegativeInt(item.props.weapon.damage, "урон")) break;
                        std::cout << "Введите прочность: "; if (!readNonNegativeInt(item.props.weapon.durability, "прочность")) break;
                        int th = 0; std::cout << "Двуручное? (0 - нет, 1 - да): ";
                        if (!readIntInRange(th, "двуручность", 0, 1)) break;
                        item.props.weapon.twoHanded = (th == 1);
                    }
                    else if (item.type == Armor) {
                        std::cout << "Введите защиту: "; if (!readNonNegativeInt(item.props.armor.defense, "защита")) break;
                        std::cout << "Введите прочность: "; if (!readNonNegativeInt(item.props.armor.durability, "прочность")) break;
                        int mg = 0; std::cout << "Магическое? (0 - нет, 1 - да): ";
                        if (!readIntInRange(mg, "магичность", 0, 1)) break;
                        item.props.armor.magical = (mg == 1);
                    }
                    else {
                        std::cout << "Введите лечение HP: "; if (!readNonNegativeInt(item.props.potion.healAmount, "лечение")) break;
                        std::cout << "Введите восстановление MP: "; if (!readNonNegativeInt(item.props.potion.manaAmount, "MP")) break;
                    }
                    inventory[freeIndex] = item;
                    std::cout << "Предмет добавлен в слот " << freeIndex << ".\n";
                }
                else if (choice == 2) {
                    int index = 0;
                    std::cout << "Введите индекс слота для удаления: ";
                    if (!readIntInRange(index, "индекс слота", 0, InventorySize - 1)) break;
                    if (inventory[index].name.empty()) std::cout << "Этот слот уже пуст.\n";
                    else { inventory[index] = Item{}; std::cout << "Предмет удалён.\n"; }
                }
                else if (choice == 3) {
                    std::cout << "\n--- ИНВЕНТАРЬ ---\n";
                    for (int i = 0; i < InventorySize; ++i) {
                        if (inventory[i].name.empty()) { std::cout << "[" << i << "] пусто\n"; continue; }
                        std::cout << "[" << i << "] " << inventory[i].name << " | ";
                        if (inventory[i].type == Weapon) std::cout << "Weapon | урон: " << inventory[i].props.weapon.damage;
                        else if (inventory[i].type == Armor) std::cout << "Armor | защита: " << inventory[i].props.armor.defense;
                        else std::cout << "Potion | лечение: " << inventory[i].props.potion.healAmount;
                        std::cout << "\n";
                    }
                }
                else { back = true; }
                if (choice != 0 && !back) pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 3: { // Экипировка
            bool back = false;
            while (!back) {
                std::cout << "\n----- ЭКИПИРОВКА ГЕРОЕВ -----\n";
                std::cout << "1. Надеть предмет\n2. Снять предмет\n3. Показать экипировку\n0. Назад\nВаш выбор: ";
                int choice = 0;
                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {
                    if (heroes.empty()) { std::cout << "Нет героев.\n"; break; }
                    int hIdx = 0; std::cout << "Индекс героя: ";
                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) break;
                    int iIdx = 0; std::cout << "Индекс предмета: ";
                    if (!readIntInRange(iIdx, "индекс предмета", 0, InventorySize - 1)) break;

                    if (inventory[iIdx].name.empty()) { std::cout << "Слот инвентаря пуст.\n"; break; }
                    int freeSlot = -1;
                    for (int i = 0; i < EquipmentSize; ++i) if (heroes[hIdx].equipment[i].name.empty()) { freeSlot = i; break; }

                    if (freeSlot == -1) std::cout << "Все 4 слота заняты.\n";
                    else {
                        heroes[hIdx].equipment[freeSlot] = inventory[iIdx];
                        inventory[iIdx] = Item{};
                        heroes[hIdx].equippedCount++;
                        std::cout << "Предмет экипирован в слот " << freeSlot << ".\n";
                    }
                }
                else if (choice == 2) {
                    if (heroes.empty()) { std::cout << "Нет героев.\n"; break; }
                    int hIdx = 0; std::cout << "Индекс героя: ";
                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) break;
                    int eIdx = 0; std::cout << "Слот экипировки (0-3): ";
                    if (!readIntInRange(eIdx, "слот", 0, EquipmentSize - 1)) break;

                    if (heroes[hIdx].equipment[eIdx].name.empty()) { std::cout << "Слот пуст.\n"; break; }
                    int freeInv = -1;
                    for (int i = 0; i < InventorySize; ++i) if (inventory[i].name.empty()) { freeInv = i; break; }

                    if (freeInv == -1) std::cout << "В инвентаре нет места.\n";
                    else {
                        inventory[freeInv] = heroes[hIdx].equipment[eIdx];
                        heroes[hIdx].equipment[eIdx] = Item{};
                        heroes[hIdx].equippedCount--;
                        std::cout << "Предмет снят в слот " << freeInv << ".\n";
                    }
                }
                else if (choice == 3) {
                    if (heroes.empty()) { std::cout << "Нет героев.\n"; break; }
                    int hIdx = 0; std::cout << "Индекс героя: ";
                    if (!readIntInRange(hIdx, "индекс героя", 0, static_cast<int>(heroes.size()) - 1)) break;
                    std::cout << "\nЭкипировка " << heroes[hIdx].name << ":\n";
                    for (int i = 0; i < EquipmentSize; ++i) {
                        if (heroes[hIdx].equipment[i].name.empty()) std::cout << "[" << i << "] пусто\n";
                        else std::cout << "[" << i << "] " << heroes[hIdx].equipment[i].name << "\n";
                    }
                }
                else { back = true; }
                if (choice != 0 && !back) pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 4: { // Враги
            bool back = false;
            while (!back) {
                std::cout << "\n----- УПРАВЛЕНИЕ ВРАГАМИ -----\n";
                std::cout << "1. Добавить врага\n2. Удалить врага\n3. Показать врагов\n0. Назад\nВаш выбор: ";
                int choice = 0;
                if (!readIntInRange(choice, "пункт меню", 0, 3)) { pauseAndClear(); continue; }

                if (choice == 1) {
                    char nameBuffer[100]{};
                    std::cout << "Введите имя врага: ";
                    std::cin.getline(nameBuffer, sizeof(nameBuffer));
                    std::string eName(nameBuffer);
                    if (eName.find_first_not_of(" \t\r\n") == std::string::npos) { std::cout << "Имя не может быть пустым.\n"; break; }

                    int hp = 0, dmg = 0;
                    std::cout << "Введите HP: "; if (!readPositiveInt(hp, "HP")) break;
                    std::cout << "Введите урон: "; if (!readPositiveInt(dmg, "урон")) break;

                    Enemy* newEnemies = new Enemy[enemyCount + 1];
                    for (int i = 0; i < enemyCount; ++i) newEnemies[i] = enemies[i];
                    newEnemies[enemyCount] = { eName, hp, dmg };
                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount++;
                    std::cout << "Враг добавлен. Индекс: " << enemyCount - 1 << "\n";
                }
                else if (choice == 2) {
                    if (enemyCount == 0) { std::cout << "Список пуст.\n"; break; }
                    int idx = 0; std::cout << "Индекс врага: ";
                    if (!readIntInRange(idx, "индекс", 0, enemyCount - 1)) break;

                    Enemy* newEnemies = nullptr;
                    if (enemyCount > 1) {
                        newEnemies = new Enemy[enemyCount - 1];
                        int nIdx = 0;
                        for (int i = 0; i < enemyCount; ++i) if (i != idx) newEnemies[nIdx++] = enemies[i];
                    }
                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount--;
                    std::cout << "Враг удалён.\n";
                }
                else if (choice == 3) {
                    if (enemyCount == 0) { std::cout << "Список пуст.\n"; break; }
                    for (int i = 0; i < enemyCount; ++i) {
                        std::cout << "[" << i << "] " << enemies[i].name << " | HP: " << enemies[i].hp << " | урон: " << enemies[i].damage << "\n";
                    }
                }
                else { back = true; }
                if (choice != 0 && !back) pauseAndClear();
            }
            clearConsole();
            break;
        }
        case 5: { // Битва
            if (heroes.empty() || enemyCount == 0) { std::cout << "Нужен хотя бы 1 герой и 1 враг.\n"; pauseAndClear(); break; }

            int turn = 1;
            while (true) {
                bool anyHeroAlive = false, anyEnemyAlive = false;
                for (const auto& h : heroes) if (h.hp > 0) anyHeroAlive = true;
                for (int i = 0; i < enemyCount; ++i) if (enemies[i].hp > 0) anyEnemyAlive = true;
                if (!anyHeroAlive || !anyEnemyAlive) break;

                std::cout << "\n========== ХОД " << turn << " ==========\n";

                // 1. Статусы
                for (auto& h : heroes) {
                    if (h.hp <= 0) continue;
                    if (h.status.poisoned) {
                        h.hp -= 3; if (h.hp < 0) h.hp = 0;
                        std::cout << h.name << " получает 3 урона от Poisoned. HP: " << h.hp << "\n";
                    }
                }

                // 2. Ход героев
                for (size_t hIdx = 0; hIdx < heroes.size(); ++hIdx) {
                    Hero& hero = heroes[hIdx];
                    if (hero.hp <= 0) continue;

                    anyEnemyAlive = false;
                    for (int i = 0; i < enemyCount; ++i) if (enemies[i].hp > 0) anyEnemyAlive = true;
                    if (!anyEnemyAlive) break;

                    std::cout << "\nХод героя: " << hero.name << "\n";

                    // Зелье
                    if (!hero.equipment[3].name.empty() && hero.equipment[3].type == Potion) {
                        int drink = 0;
                        std::cout << "В 4-м слоте зелье. Выпить? (1 - да, 0 - нет): ";
                        if (readIntInRange(drink, "выбор", 0, 1) && drink == 1) {
                            hero.hp += hero.equipment[3].props.potion.healAmount;
                            hero.mp += hero.equipment[3].props.potion.manaAmount;
                            if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
                            if (hero.mp > hero.maxMp) hero.mp = hero.maxMp;
                            hero.equipment[3] = Item{};
                            hero.equippedCount--;
                            std::cout << "Зелье использовано.\n";
                        }
                    }

                    int useAbility = 0;
                    std::cout << "Использовать способность? (1 - да, 0 - нет): ";
                    if (!readIntInRange(useAbility, "способность", 0, 1)) continue;

                    int abilityCost = (hero.heroClass == Warrior) ? 20 : 40;
                    bool canAbility = (useAbility == 1 && hero.mp >= abilityCost);
                    if (useAbility == 1 && !canAbility) std::cout << "Недостаточно MP.\n";

                    int weaponDmg = 0;
                    for (int i = 0; i < EquipmentSize; ++i)
                        if (!hero.equipment[i].name.empty() && hero.equipment[i].type == Weapon)
                            weaponDmg += hero.equipment[i].props.weapon.damage;

                    if (hero.heroClass == Healer && canAbility) {
                        int allyIdx = 0;
                        std::cout << "Индекс союзника для лечения: ";
                        if (readIntInRange(allyIdx, "союзник", 0, static_cast<int>(heroes.size()) - 1)) {
                            if (heroes[allyIdx].hp > 0) {
                                hero.mp -= abilityCost;
                                int heal = 6 + weaponDmg;
                                heroes[allyIdx].hp += heal;
                                if (heroes[allyIdx].hp > heroes[allyIdx].maxHp) heroes[allyIdx].hp = heroes[allyIdx].maxHp;
                                std::cout << hero.name << " лечит " << heroes[allyIdx].name << " на " << heal << " HP.\n";
                            }
                            else { std::cout << "Союзник мёртв.\n"; }
                        }
                    }
                    else {
                        int targetIdx = 0;
                        std::cout << "Индекс врага: ";
                        if (!readIntInRange(targetIdx, "враг", 0, enemyCount - 1)) continue;
                        if (enemies[targetIdx].hp <= 0) { std::cout << "Враг мёртв.\n"; continue; }

                        if (canAbility) hero.mp -= abilityCost;

                        double dmg = 0;
                        if (hero.heroClass == Warrior) dmg = canAbility ? (15.0 * 1.5) : (15.0 + weaponDmg);
                        else if (hero.heroClass == Mage) dmg = canAbility ? (8.0 * 3.0) : (8.0 + weaponDmg);
                        else dmg = 6.0 + weaponDmg;

                        if (hero.status.blessed) dmg *= 2.0;
                        if (hero.status.cursed) dmg = std::ceil(dmg / 2.0);

                        int finalDmg = static_cast<int>(dmg);
                        if (finalDmg < 1) finalDmg = 1;

                        enemies[targetIdx].hp -= finalDmg;
                        if (enemies[targetIdx].hp < 0) enemies[targetIdx].hp = 0;
                        std::cout << hero.name << " наносит " << finalDmg << " урона. HP врага: " << enemies[targetIdx].hp << "\n";
                    }
                }

                // 3. Ход врагов
                anyEnemyAlive = false;
                for (int i = 0; i < enemyCount; ++i) if (enemies[i].hp > 0) anyEnemyAlive = true;
                if (anyEnemyAlive) {
                    for (int i = 0; i < enemyCount; ++i) {
                        if (enemies[i].hp <= 0) continue;
                        int targetIdx = 0;
                        std::cout << "\nВраг " << enemies[i].name << " атакует. Индекс героя: ";
                        if (!readIntInRange(targetIdx, "герой", 0, static_cast<int>(heroes.size()) - 1)) continue;
                        if (heroes[targetIdx].hp <= 0) { std::cout << "Герой мёртв.\n"; continue; }

                        int armorDef = 0;
                        for (int slot = 0; slot < EquipmentSize; ++slot)
                            if (!heroes[targetIdx].equipment[slot].name.empty() && heroes[targetIdx].equipment[slot].type == Armor)
                                armorDef += heroes[targetIdx].equipment[slot].props.armor.defense;

                        int dmg = enemies[i].damage - armorDef;
                        if (dmg < 1) dmg = 1;

                        heroes[targetIdx].hp -= dmg;
                        if (heroes[targetIdx].hp < 0) heroes[targetIdx].hp = 0;
                        std::cout << enemies[i].name << " наносит " << dmg << " урона. HP героя: " << heroes[targetIdx].hp << "\n";
                    }
                }

                // 4. Состояние
                std::cout << "\n--- СОСТОЯНИЕ ПОСЛЕ ХОДА ---\n";
                for (size_t i = 0; i < heroes.size(); ++i) {
                    std::cout << "[" << i << "] " << heroes[i].name << " | " << (heroes[i].hp > 0 ? "жив" : "мёртв")
                        << " | HP " << heroes[i].hp << "/" << heroes[i].maxHp << " | MP " << heroes[i].mp << "/" << heroes[i].maxMp << "\n";
                }
                ++turn;
            }

            bool anyHeroAlive = false, anyEnemyAlive = false;
            for (const auto& h : heroes) if (h.hp > 0) anyHeroAlive = true;
            for (int i = 0; i < enemyCount; ++i) if (enemies[i].hp > 0) anyEnemyAlive = true;

            std::cout << "\n========== РЕЗУЛЬТАТ БИТВЫ ==========\n";
            if (anyHeroAlive && !anyEnemyAlive) std::cout << "Победа героев!\n";
            else if (!anyHeroAlive && anyEnemyAlive) std::cout << "Победа врагов.\n";
            else std::cout << "Ничья.\n";
            pauseAndClear();
            break;
        }
        case 6: { // Сводка
            std::cout << "\n========== СВОДКА ПО ОТРЯДУ ==========\n";
            long long totalLevel = 0, totalHp = 0, totalMp = 0;
            for (const auto& h : heroes) { totalLevel += h.level; totalHp += h.hp; totalMp += h.mp; }

            std::cout << "Героев: " << heroes.size() << " | Сумм. уровень: " << totalLevel << " | Сумм. HP: " << totalHp << " | Сумм. MP: " << totalMp << "\n\n";

            for (size_t i = 0; i < heroes.size(); ++i) {
                std::cout << "[" << i << "] " << heroes[i].name << " | HP " << heroes[i].hp << "/" << heroes[i].maxHp << " | MP " << heroes[i].mp << "/" << heroes[i].maxMp << " | статусы: ";
                printStatusFlags(heroes[i].status);
                std::cout << "\n  Экипировка: ";
                bool hasEq = false;
                for (int slot = 0; slot < EquipmentSize; ++slot) {
                    if (!heroes[i].equipment[slot].name.empty()) {
                        if (hasEq) std::cout << ", ";
                        std::cout << heroes[i].equipment[slot].name;
                        hasEq = true;
                    }
                }
                if (!hasEq) std::cout << "нет";
                std::cout << "\n";
            }

            int usedSlots = 0; long long totalValue = 0, totalWeight = 0;
            for (const auto& item : inventory) {
                if (!item.name.empty()) { ++usedSlots; totalValue += item.value; totalWeight += item.weight; }
            }
            std::cout << "\nИнвентарь: Занято " << usedSlots << "/16 | Стоимость: " << totalValue << " | Вес: " << totalWeight << "\n";

            std::cout << "\nВраги:\n";
            if (enemyCount == 0) std::cout << "Нет врагов.\n";
            else for (int i = 0; i < enemyCount; ++i) std::cout << "[" << i << "] " << enemies[i].name << " | HP: " << enemies[i].hp << " | урон: " << enemies[i].damage << "\n";

            pauseAndClear();
            break;
        }
        }
    }

    // Очистка памяти
    for (auto& hero : heroes) {
        delete[] hero.equipment;
        hero.equipment = nullptr;
    }
    heroes.clear();
    delete[] enemies;
    enemies = nullptr;

    clearConsole();
    std::cout << "Программа завершена.\n";
    return 0;
}