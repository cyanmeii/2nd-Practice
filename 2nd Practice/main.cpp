#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <limits>

using namespace std;

enum HeroClass {
    Warrior,
    Mage,
    Healer
};

enum ItemType {
    Weapon,
    Armor,
    Potion
};

enum StatusEffect {
    None,
    Blessed,
    Cursed,
    Poisoned
};

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
    string name;
    ItemType type;
    int weight;
    int value;
    ItemProperties props;
};

struct Hero {
    string name;
    HeroClass heroClass;
    int level;
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    StatusFlags status{};
    Item* equipment;
    int equippedCount;
};

struct Enemy {
    string name;
    int hp;
    int damage;
};

int main() {
    array<Item, 16> inventory{};
    vector<Hero> heroes;
    Enemy* enemies = nullptr;
    int enemyCount = 0;

    for (int i = 0; i < 16; ++i) {
        inventory[i].name = "";
    }

    bool running = true;

    while (running) {
        cout << "\n========== ГЛАВНОЕ МЕНЮ ==========" << endl;
        cout << "1. Управление отрядом" << endl;
        cout << "2. Управление инвентарём" << endl;
        cout << "3. Экипировка героев" << endl;
        cout << "4. Управление врагами" << endl;
        cout << "5. Симуляция битвы" << endl;
        cout << "6. Сводка по отряду" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";

        int mainChoice;
        cin >> mainChoice;

        switch (mainChoice) {
        case 1: {
            bool back = false;
            while (!back) {
                cout << "\n----- УПРАВЛЕНИЕ ОТРЯДОМ -----" << endl;
                cout << "1. Добавить героя" << endl;
                cout << "2. Удалить героя" << endl;
                cout << "3. Показать всех героев" << endl;
                cout << "0. Назад" << endl;
                cout << "Ваш выбор: ";

                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    Hero hero{};

                    cout << "Введите имя героя: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, hero.name);

                    int classChoice;
                    cout << "Выберите класс (1 - Warrior, 2 - Mage, 3 - Healer): ";
                    cin >> classChoice;
                    while (classChoice < 1 || classChoice > 3) {
                        cout << "Некорректный класс. Повторите: ";
                        cin >> classChoice;
                    }

                    switch (classChoice) {
                    case 1:
                        hero.heroClass = Warrior;
                        break;
                    case 2:
                        hero.heroClass = Mage;
                        break;
                    case 3:
                        hero.heroClass = Healer;
                        break;
                    }

                    cout << "Введите уровень: ";
                    cin >> hero.level;
                    while (hero.level < 1) {
                        cout << "Уровень должен быть не меньше 1. Повторите: ";
                        cin >> hero.level;
                    }

                    switch (hero.heroClass) {
                    case Warrior:
                        hero.maxHp = 100 + hero.level * 20;
                        hero.maxMp = 10 + hero.level * 2;
                        break;
                    case Mage:
                        hero.maxHp = 50 + hero.level * 8;
                        hero.maxMp = 100 + hero.level * 25;
                        break;
                    case Healer:
                        hero.maxHp = 60 + hero.level * 10;
                        hero.maxMp = 80 + hero.level * 20;
                        break;
                    }

                    hero.hp = hero.maxHp;
                    hero.mp = hero.maxMp;

                    int statusChoice;
                    cout << "Выберите статус (0 - None, 1 - Blessed, 2 - Cursed, 3 - Poisoned): ";
                    cin >> statusChoice;
                    while (statusChoice < 0 || statusChoice > 3) {
                        cout << "Некорректный статус. Повторите: ";
                        cin >> statusChoice;
                    }

                    hero.status.blessed = 0;
                    hero.status.cursed = 0;
                    hero.status.poisoned = 0;

                    switch (static_cast<StatusEffect>(statusChoice)) {
                    case Blessed:
                        hero.status.blessed = 1;
                        break;
                    case Cursed:
                        hero.status.cursed = 1;
                        break;
                    case Poisoned:
                        hero.status.poisoned = 1;
                        break;
                    case None:
                        break;
                    }

                    hero.equipment = new Item[4];
                    hero.equippedCount = 0;
                    for (int i = 0; i < 4; ++i) {
                        hero.equipment[i].name = "";
                    }

                    heroes.push_back(hero);
                    cout << "Герой добавлен. Индекс: " << heroes.size() - 1 << endl;
                    break;
                }

                case 2: {
                    if (heroes.empty()) {
                        cout << "Отряд пуст." << endl;
                        break;
                    }

                    int index;
                    cout << "Введите индекс героя для удаления: ";
                    cin >> index;

                    if (index < 0 || index >= static_cast<int>(heroes.size())) {
                        cout << "Некорректный индекс." << endl;
                        break;
                    }

                    delete[] heroes[index].equipment;
                    heroes.erase(heroes.begin() + index);
                    cout << "Герой удалён." << endl;
                    break;
                }

                case 3: {
                    if (heroes.empty()) {
                        cout << "Отряд пуст." << endl;
                        break;
                    }

                    cout << "\nСписок героев:" << endl;
                    for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                        Hero& hero = heroes[i];

                        cout << "[" << i << "] " << hero.name << " | ";
                        switch (hero.heroClass) {
                        case Warrior:
                            cout << "Warrior";
                            break;
                        case Mage:
                            cout << "Mage";
                            break;
                        case Healer:
                            cout << "Healer";
                            break;
                        }

                        cout << " | Lv." << hero.level
                            << " | HP: " << hero.hp << "/" << hero.maxHp
                            << " | MP: " << hero.mp << "/" << hero.maxMp
                            << " | Статусы: ";

                        bool hasStatus = false;
                        if (hero.status.blessed) {
                            cout << "Blessed ";
                            hasStatus = true;
                        }
                        if (hero.status.cursed) {
                            cout << "Cursed ";
                            hasStatus = true;
                        }
                        if (hero.status.poisoned) {
                            cout << "Poisoned ";
                            hasStatus = true;
                        }
                        if (!hasStatus) {
                            cout << "None";
                        }
                        cout << endl;
                    }
                    break;
                }

                case 0:
                    back = true;
                    break;

                default:
                    cout << "Некорректный пункт." << endl;
                }
            }
            break;
        }

        case 2: {
            bool back = false;
            while (!back) {
                cout << "\n----- УПРАВЛЕНИЕ ИНВЕНТАРЁМ -----" << endl;
                cout << "1. Добавить предмет" << endl;
                cout << "2. Удалить предмет" << endl;
                cout << "3. Показать инвентарь" << endl;
                cout << "0. Назад" << endl;
                cout << "Ваш выбор: ";

                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    int freeIndex = -1;
                    for (int i = 0; i < 16; ++i) {
                        if (inventory[i].name.empty()) {
                            freeIndex = i;
                            break;
                        }
                    }

                    if (freeIndex == -1) {
                        cout << "Инвентарь заполнен." << endl;
                        break;
                    }

                    Item item{};
                    cout << "Введите название предмета: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, item.name);

                    int typeChoice;
                    cout << "Выберите тип (1 - Weapon, 2 - Armor, 3 - Potion): ";
                    cin >> typeChoice;
                    while (typeChoice < 1 || typeChoice > 3) {
                        cout << "Некорректный тип. Повторите: ";
                        cin >> typeChoice;
                    }

                    switch (typeChoice) {
                    case 1:
                        item.type = Weapon;
                        break;
                    case 2:
                        item.type = Armor;
                        break;
                    case 3:
                        item.type = Potion;
                        break;
                    }

                    cout << "Введите вес: ";
                    cin >> item.weight;
                    cout << "Введите стоимость: ";
                    cin >> item.value;

                    switch (item.type) {
                    case Weapon: {
                        int twoHanded;
                        cout << "Урон: ";
                        cin >> item.props.weapon.damage;
                        cout << "Прочность: ";
                        cin >> item.props.weapon.durability;
                        cout << "Двуручное? (0/1): ";
                        cin >> twoHanded;
                        item.props.weapon.twoHanded = (twoHanded != 0);
                        break;
                    }
                    case Armor: {
                        int magical;
                        cout << "Защита: ";
                        cin >> item.props.armor.defense;
                        cout << "Прочность: ";
                        cin >> item.props.armor.durability;
                        cout << "Магическое? (0/1): ";
                        cin >> magical;
                        item.props.armor.magical = (magical != 0);
                        break;
                    }
                    case Potion:
                        cout << "Лечение HP: ";
                        cin >> item.props.potion.healAmount;
                        cout << "Восстановление MP: ";
                        cin >> item.props.potion.manaAmount;
                        break;
                    }

                    inventory[freeIndex] = item;
                    cout << "Предмет добавлен в слот " << freeIndex << "." << endl;
                    break;
                }

                case 2: {
                    int index;
                    cout << "Введите индекс слота для удаления (0-15): ";
                    cin >> index;
                    if (index < 0 || index >= 16) {
                        cout << "Некорректный индекс." << endl;
                    }
                    else if (inventory[index].name.empty()) {
                        cout << "Слот уже пуст." << endl;
                    }
                    else {
                        inventory[index].name = "";
                        cout << "Предмет удалён." << endl;
                    }
                    break;
                }

                case 3: {
                    cout << "\n----- ИНВЕНТАРЬ -----" << endl;
                    for (int i = 0; i < 16; ++i) {
                        if (inventory[i].name.empty()) {
                            cout << "[" << i << "] Пусто" << endl;
                            continue;
                        }

                        Item& item = inventory[i];
                        cout << "[" << i << "] " << item.name << " | ";

                        switch (item.type) {
                        case Weapon:
                            cout << "Weapon"
                                << " | weight=" << item.weight
                                << " | value=" << item.value
                                << " | damage=" << item.props.weapon.damage
                                << " | durability=" << item.props.weapon.durability
                                << " | twoHanded=" << item.props.weapon.twoHanded;
                            break;
                        case Armor:
                            cout << "Armor"
                                << " | weight=" << item.weight
                                << " | value=" << item.value
                                << " | defense=" << item.props.armor.defense
                                << " | durability=" << item.props.armor.durability
                                << " | magical=" << item.props.armor.magical;
                            break;
                        case Potion:
                            cout << "Potion"
                                << " | weight=" << item.weight
                                << " | value=" << item.value
                                << " | healAmount=" << item.props.potion.healAmount
                                << " | manaAmount=" << item.props.potion.manaAmount;
                            break;
                        }
                        cout << endl;
                    }
                    break;
                }

                case 0:
                    back = true;
                    break;

                default:
                    cout << "Некорректный пункт." << endl;
                }
            }
            break;
        }

        case 3: {
            bool back = false;
            while (!back) {
                cout << "\n----- ЭКИПИРОВКА ГЕРОЕВ -----" << endl;
                cout << "1. Надеть предмет" << endl;
                cout << "2. Снять предмет" << endl;
                cout << "3. Показать экипировку героя" << endl;
                cout << "0. Назад" << endl;
                cout << "Ваш выбор: ";

                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    if (heroes.empty()) {
                        cout << "Нет героев." << endl;
                        break;
                    }

                    int heroIndex;
                    int itemIndex;
                    cout << "Индекс героя: ";
                    cin >> heroIndex;
                    cout << "Индекс предмета в инвентаре: ";
                    cin >> itemIndex;

                    if (heroIndex < 0 || heroIndex >= static_cast<int>(heroes.size()) ||
                        itemIndex < 0 || itemIndex >= 16) {
                        cout << "Некорректный индекс." << endl;
                        break;
                    }

                    if (inventory[itemIndex].name.empty()) {
                        cout << "Этот слот инвентаря пуст." << endl;
                        break;
                    }

                    int equipmentIndex = -1;
                    for (int i = 0; i < 4; ++i) {
                        if (heroes[heroIndex].equipment[i].name.empty()) {
                            equipmentIndex = i;
                            break;
                        }
                    }

                    if (equipmentIndex == -1) {
                        cout << "Все 4 слота экипировки заняты." << endl;
                        break;
                    }

                    heroes[heroIndex].equipment[equipmentIndex] = inventory[itemIndex];
                    inventory[itemIndex].name = "";
                    heroes[heroIndex].equippedCount++;

                    cout << "Предмет экипирован в слот " << equipmentIndex << "." << endl;
                    break;
                }

                case 2: {
                    if (heroes.empty()) {
                        cout << "Нет героев." << endl;
                        break;
                    }

                    int heroIndex;
                    int equipmentIndex;
                    cout << "Индекс героя: ";
                    cin >> heroIndex;
                    cout << "Номер слота экипировки (0-3): ";
                    cin >> equipmentIndex;

                    if (heroIndex < 0 || heroIndex >= static_cast<int>(heroes.size()) ||
                        equipmentIndex < 0 || equipmentIndex >= 4) {
                        cout << "Некорректный индекс." << endl;
                        break;
                    }

                    if (heroes[heroIndex].equipment[equipmentIndex].name.empty()) {
                        cout << "Слот экипировки пуст." << endl;
                        break;
                    }

                    int freeInventoryIndex = -1;
                    for (int i = 0; i < 16; ++i) {
                        if (inventory[i].name.empty()) {
                            freeInventoryIndex = i;
                            break;
                        }
                    }

                    if (freeInventoryIndex == -1) {
                        cout << "Нет свободного места в инвентаре. Предмет пока остаётся экипирован." << endl;
                        break;
                    }

                    inventory[freeInventoryIndex] = heroes[heroIndex].equipment[equipmentIndex];
                    heroes[heroIndex].equipment[equipmentIndex].name = "";
                    heroes[heroIndex].equippedCount--;

                    cout << "Предмет снят и помещён в слот " << freeInventoryIndex << "." << endl;
                    break;
                }

                case 3: {
                    if (heroes.empty()) {
                        cout << "Нет героев." << endl;
                        break;
                    }

                    int heroIndex;
                    cout << "Индекс героя: ";
                    cin >> heroIndex;

                    if (heroIndex < 0 || heroIndex >= static_cast<int>(heroes.size())) {
                        cout << "Некорректный индекс." << endl;
                        break;
                    }

                    cout << "Экипировка героя " << heroes[heroIndex].name << ":" << endl;
                    bool empty = true;
                    for (int i = 0; i < 4; ++i) {
                        if (heroes[heroIndex].equipment[i].name.empty()) {
                            cout << "[" << i << "] Пусто" << endl;
                        }
                        else {
                            cout << "[" << i << "] " << heroes[heroIndex].equipment[i].name << " | ";
                            switch (heroes[heroIndex].equipment[i].type) {
                            case Weapon:
                                cout << "Weapon, damage=" << heroes[heroIndex].equipment[i].props.weapon.damage;
                                break;
                            case Armor:
                                cout << "Armor, defense=" << heroes[heroIndex].equipment[i].props.armor.defense;
                                break;
                            case Potion:
                                cout << "Potion, heal=" << heroes[heroIndex].equipment[i].props.potion.healAmount
                                    << ", mana=" << heroes[heroIndex].equipment[i].props.potion.manaAmount;
                                break;
                            }
                            cout << endl;
                            empty = false;
                        }
                    }

                    if (empty) {
                        cout << "Экипировка полностью пуста." << endl;
                    }
                    break;
                }

                case 0:
                    back = true;
                    break;

                default:
                    cout << "Некорректный пункт." << endl;
                }
            }
            break;
        }

        case 4: {
            bool back = false;
            while (!back) {
                cout << "\n----- УПРАВЛЕНИЕ ВРАГАМИ -----" << endl;
                cout << "1. Добавить врага" << endl;
                cout << "2. Удалить врага" << endl;
                cout << "3. Показать всех врагов" << endl;
                cout << "0. Назад" << endl;
                cout << "Ваш выбор: ";

                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    Enemy* newEnemies = new Enemy[enemyCount + 1];

                    for (int i = 0; i < enemyCount; ++i) {
                        newEnemies[i] = enemies[i];
                    }

                    char nameBuffer[100];
                    cout << "Введите имя врага: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.getline(nameBuffer, 100);

                    newEnemies[enemyCount].name = string(nameBuffer);
                    cout << "Введите HP врага: ";
                    cin >> newEnemies[enemyCount].hp;
                    cout << "Введите урон врага: ";
                    cin >> newEnemies[enemyCount].damage;

                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount++;

                    cout << "Враг добавлен. Индекс: " << enemyCount - 1 << endl;
                    break;
                }

                case 2: {
                    if (enemyCount == 0) {
                        cout << "Список врагов пуст." << endl;
                        break;
                    }

                    int index;
                    cout << "Введите индекс врага: ";
                    cin >> index;

                    if (index < 0 || index >= enemyCount) {
                        cout << "Некорректный индекс." << endl;
                        break;
                    }

                    if (enemyCount == 1) {
                        delete[] enemies;
                        enemies = nullptr;
                        enemyCount = 0;
                        cout << "Враг удалён." << endl;
                        break;
                    }

                    Enemy* newEnemies = new Enemy[enemyCount - 1];
                    int newIndex = 0;
                    for (int i = 0; i < enemyCount; ++i) {
                        if (i != index) {
                            newEnemies[newIndex] = enemies[i];
                            newIndex++;
                        }
                    }

                    delete[] enemies;
                    enemies = newEnemies;
                    enemyCount--;
                    cout << "Враг удалён." << endl;
                    break;
                }

                case 3: {
                    if (enemyCount == 0) {
                        cout << "Список врагов пуст." << endl;
                        break;
                    }

                    cout << "\nСписок врагов:" << endl;
                    for (int i = 0; i < enemyCount; ++i) {
                        cout << "[" << i << "] "
                            << enemies[i].name
                            << " | HP=" << enemies[i].hp
                            << " | damage=" << enemies[i].damage << endl;
                    }
                    break;
                }

                case 0:
                    back = true;
                    break;

                default:
                    cout << "Некорректный пункт." << endl;
                }
            }
            break;
        }

        case 5: {
            if (heroes.empty()) {
                cout << "Для боя нужен хотя бы один герой." << endl;
                break;
            }
            if (enemyCount == 0) {
                cout << "Для боя нужен хотя бы один враг." << endl;
                break;
            }

            cout << "\n========== НАЧАЛО БИТВЫ ==========" << endl;
            cout << "Герои:" << endl;
            for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                cout << "[" << i << "] " << heroes[i].name
                    << " HP=" << heroes[i].hp << "/" << heroes[i].maxHp
                    << " MP=" << heroes[i].mp << "/" << heroes[i].maxMp << endl;
            }

            cout << "Враги:" << endl;
            for (int i = 0; i < enemyCount; ++i) {
                cout << "[" << i << "] " << enemies[i].name
                    << " HP=" << enemies[i].hp
                    << " damage=" << enemies[i].damage << endl;
            }

            int turn = 1;

            while (true) {
                bool anyHeroAlive = false;
                for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                    if (heroes[i].hp > 0) {
                        anyHeroAlive = true;
                        break;
                    }
                }

                bool anyEnemyAlive = false;
                for (int i = 0; i < enemyCount; ++i) {
                    if (enemies[i].hp > 0) {
                        anyEnemyAlive = true;
                        break;
                    }
                }

                if (!anyHeroAlive || !anyEnemyAlive) {
                    break;
                }

                cout << "\n========== ХОД " << turn << " ==========" << endl;

                // 1. Статусные эффекты героев
                for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                    if (heroes[i].hp <= 0) {
                        continue;
                    }

                    if (heroes[i].status.poisoned) {
                        heroes[i].hp -= 3;
                        if (heroes[i].hp < 0) {
                            heroes[i].hp = 0;
                        }
                        cout << heroes[i].name << " получает 3 урона от Poisoned." << endl;
                    }
                }

                // Проверка после отравления
                anyHeroAlive = false;
                for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                    if (heroes[i].hp > 0) {
                        anyHeroAlive = true;
                        break;
                    }
                }
                if (!anyHeroAlive) {
                    break;
                }

                // 2. Ход героев
                for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                    if (heroes[i].hp <= 0) {
                        continue;
                    }

                    anyEnemyAlive = false;
                    for (int j = 0; j < enemyCount; ++j) {
                        if (enemies[j].hp > 0) {
                            anyEnemyAlive = true;
                            break;
                        }
                    }
                    if (!anyEnemyAlive) {
                        break;
                    }

                    // 3. Использование зелья из 4-го слота (индекс 3)
                    if (!heroes[i].equipment[3].name.empty() &&
                        heroes[i].equipment[3].type == Potion) {
                        cout << heroes[i].name << " имеет зелье в 4-м слоте. Выпить? (1 - да, 0 - нет): ";
                        int drink;
                        cin >> drink;
                        if (drink == 1) {
                            int heal = heroes[i].equipment[3].props.potion.healAmount;
                            int mana = heroes[i].equipment[3].props.potion.manaAmount;
                            heroes[i].hp += heal;
                            heroes[i].mp += mana;
                            if (heroes[i].hp > heroes[i].maxHp) {
                                heroes[i].hp = heroes[i].maxHp;
                            }
                            if (heroes[i].mp > heroes[i].maxMp) {
                                heroes[i].mp = heroes[i].maxMp;
                            }

                            heroes[i].equipment[3].name = "";
                            heroes[i].equippedCount--;

                            cout << "Зелье использовано. HP=" << heroes[i].hp
                                << ", MP=" << heroes[i].mp << endl;
                        }
                    }

                    int useAbility;
                    cout << heroes[i].name << ": использовать способность? (1 - да, 0 - нет): ";
                    cin >> useAbility;

                    int abilityCost = 0;
                    bool abilityAllowed = false;
                    double abilityMultiplier = 1.0;

                    if (heroes[i].heroClass == Warrior) {
                        abilityCost = 20;
                        abilityMultiplier = 1.5;
                    }
                    else if (heroes[i].heroClass == Mage) {
                        abilityCost = 40;
                        abilityMultiplier = 3.0;
                    }
                    else if (heroes[i].heroClass == Healer) {
                        abilityCost = 40;
                    }

                    if (useAbility == 1 && heroes[i].mp >= abilityCost) {
                        abilityAllowed = true;
                        heroes[i].mp -= abilityCost;
                    }
                    else if (useAbility == 1 && heroes[i].mp < abilityCost) {
                        cout << "Недостаточно MP. Выполняется обычное действие." << endl;
                    }

                    // Находим суммарный урон оружия в экипировке
                    int weaponDamage = 0;
                    for (int slot = 0; slot < 4; ++slot) {
                        if (!heroes[i].equipment[slot].name.empty() &&
                            heroes[i].equipment[slot].type == Weapon) {
                            weaponDamage += heroes[i].equipment[slot].props.weapon.damage;
                        }
                    }

                    if (heroes[i].heroClass == Healer && abilityAllowed) {
                        int healAmount = 6 + weaponDamage;
                        int allyIndex;

                        cout << "Выберите героя для лечения: ";
                        cin >> allyIndex;

                        if (allyIndex < 0 || allyIndex >= static_cast<int>(heroes.size())) {
                            cout << "Некорректный индекс. Лечение отменено." << endl;
                            heroes[i].mp += abilityCost;
                        }
                        else if (heroes[allyIndex].hp <= 0) {
                            cout << "Нельзя лечить поверженного героя. Лечение отменено." << endl;
                            heroes[i].mp += abilityCost;
                        }
                        else {
                            heroes[allyIndex].hp += healAmount;
                            if (heroes[allyIndex].hp > heroes[allyIndex].maxHp) {
                                heroes[allyIndex].hp = heroes[allyIndex].maxHp;
                            }
                            cout << heroes[i].name << " использует Лечение на "
                                << heroes[allyIndex].name << " (+" << healAmount << " HP)." << endl;
                        }
                    }
                    else {
                        int enemyIndex;
                        cout << "Выберите цель (индекс врага): ";
                        cin >> enemyIndex;

                        if (enemyIndex < 0 || enemyIndex >= enemyCount || enemies[enemyIndex].hp <= 0) {
                            cout << "Некорректная или мёртвая цель. Ход пропущен." << endl;
                            if (abilityAllowed) {
                                heroes[i].mp += abilityCost;
                            }
                            continue;
                        }

                        int baseDamage = 0;
                        if (heroes[i].heroClass == Warrior) {
                            baseDamage = 15;
                        }
                        else if (heroes[i].heroClass == Mage) {
                            baseDamage = 8;
                        }
                        else {
                            baseDamage = 6;
                        }

                        int damage;

                        if (abilityAllowed) {
                            damage = static_cast<int>(baseDamage * abilityMultiplier);
                        }
                        else {
                            damage = baseDamage + weaponDamage;
                        }

                        // Модификаторы статусов
                        if (heroes[i].status.blessed) {
                            damage *= 2;
                        }
                        if (heroes[i].status.cursed) {
                            damage = (damage + 1) / 2;
                        }

                        if (damage < 1) {
                            damage = 1;
                        }

                        enemies[enemyIndex].hp -= damage;
                        if (enemies[enemyIndex].hp < 0) {
                            enemies[enemyIndex].hp = 0;
                        }

                        cout << heroes[i].name << " наносит " << damage << " урона врагу "
                            << enemies[enemyIndex].name;
                        if (abilityAllowed) {
                            if (heroes[i].heroClass == Warrior) {
                                cout << " способностью Боевой клич";
                            }
                            else if (heroes[i].heroClass == Mage) {
                                cout << " способностью Чароплёт";
                            }
                        }
                        else {
                            cout << " обычной атакой";
                        }
                        cout << ". HP врага: " << enemies[enemyIndex].hp << endl;
                    }
                }

                // Проверка после хода героев
                anyEnemyAlive = false;
                for (int i = 0; i < enemyCount; ++i) {
                    if (enemies[i].hp > 0) {
                        anyEnemyAlive = true;
                        break;
                    }
                }
                if (!anyEnemyAlive) {
                    break;
                }

                // 4. Ход врагов
                for (int i = 0; i < enemyCount; ++i) {
                    if (enemies[i].hp <= 0) {
                        continue;
                    }

                    anyHeroAlive = false;
                    for (int j = 0; j < static_cast<int>(heroes.size()); ++j) {
                        if (heroes[j].hp > 0) {
                            anyHeroAlive = true;
                            break;
                        }
                    }
                    if (!anyHeroAlive) {
                        break;
                    }

                    int targetHero;
                    cout << "Враг " << enemies[i].name << " атакует. Выберите героя (индекс): ";
                    cin >> targetHero;

                    if (targetHero < 0 || targetHero >= static_cast<int>(heroes.size()) ||
                        heroes[targetHero].hp <= 0) {
                        cout << "Некорректная или мёртвая цель. Атака пропущена." << endl;
                        continue;
                    }

                    int armorDefense = 0;
                    for (int slot = 0; slot < 4; ++slot) {
                        if (!heroes[targetHero].equipment[slot].name.empty() &&
                            heroes[targetHero].equipment[slot].type == Armor) {
                            armorDefense += heroes[targetHero].equipment[slot].props.armor.defense;
                        }
                    }

                    int damage = enemies[i].damage - armorDefense;
                    if (damage < 1) {
                        damage = 1;
                    }

                    heroes[targetHero].hp -= damage;
                    if (heroes[targetHero].hp < 0) {
                        heroes[targetHero].hp = 0;
                    }

                    cout << "Враг " << enemies[i].name << " наносит " << damage
                        << " урона герою " << heroes[targetHero].name
                        << ". HP героя: " << heroes[targetHero].hp << endl;
                }

                // 5. Вывод состояния
                cout << "\n--- Состояние после хода ---" << endl;
                cout << "Герои:" << endl;
                for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                    cout << "[" << i << "] " << heroes[i].name
                        << " | " << (heroes[i].hp > 0 ? "жив" : "мёртв")
                        << " | HP " << heroes[i].hp << "/" << heroes[i].maxHp
                        << " | MP " << heroes[i].mp << "/" << heroes[i].maxMp
                        << " | ";

                    bool hasStatus = false;
                    if (heroes[i].status.blessed) {
                        cout << "Blessed ";
                        hasStatus = true;
                    }
                    if (heroes[i].status.cursed) {
                        cout << "Cursed ";
                        hasStatus = true;
                    }
                    if (heroes[i].status.poisoned) {
                        cout << "Poisoned ";
                        hasStatus = true;
                    }
                    if (!hasStatus) {
                        cout << "None";
                    }
                    cout << endl;
                }

                cout << "Враги:" << endl;
                for (int i = 0; i < enemyCount; ++i) {
                    cout << "[" << i << "] " << enemies[i].name
                        << " | " << (enemies[i].hp > 0 ? "жив" : "мёртв")
                        << " | HP " << enemies[i].hp << endl;
                }

                ++turn;
            }

            bool anyHeroAlive = false;
            for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                if (heroes[i].hp > 0) {
                    anyHeroAlive = true;
                    break;
                }
            }

            bool anyEnemyAlive = false;
            for (int i = 0; i < enemyCount; ++i) {
                if (enemies[i].hp > 0) {
                    anyEnemyAlive = true;
                    break;
                }
            }

            cout << "\n========== КОНЕЦ БИТВЫ ==========" << endl;
            if (anyHeroAlive) {
                cout << "Победа героев!" << endl;
            }
            else if (anyEnemyAlive) {
                cout << "Победа врагов!" << endl;
            }
            else {
                cout << "Все участники повержены одновременно." << endl;
            }
            break;
        }

        case 6: {
            cout << "\n========== СВОДКА ПО ОТРЯДУ ==========" << endl;

            int totalLevel = 0;
            int totalHp = 0;
            int totalMp = 0;

            cout << "Количество героев: " << heroes.size() << endl;
            for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                totalLevel += heroes[i].level;
                totalHp += heroes[i].hp;
                totalMp += heroes[i].mp;
            }

            cout << "Суммарный уровень: " << totalLevel << endl;
            cout << "Суммарное HP: " << totalHp << endl;
            cout << "Суммарное MP: " << totalMp << endl;

            cout << "\nГерои, экипировка и статусы:" << endl;
            for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
                cout << "[" << i << "] " << heroes[i].name << " | " << heroes[i].hp
                    << "/" << heroes[i].maxHp << " HP | "
                    << heroes[i].mp << "/" << heroes[i].maxMp << " MP" << endl;

                cout << "  Статусы: ";
                bool hasStatus = false;
                if (heroes[i].status.blessed) {
                    cout << "Blessed ";
                    hasStatus = true;
                }
                if (heroes[i].status.cursed) {
                    cout << "Cursed ";
                    hasStatus = true;
                }
                if (heroes[i].status.poisoned) {
                    cout << "Poisoned ";
                    hasStatus = true;
                }
                if (!hasStatus) {
                    cout << "None";
                }
                cout << endl;

                cout << "  Экипировка:" << endl;
                for (int slot = 0; slot < 4; ++slot) {
                    if (heroes[i].equipment[slot].name.empty()) {
                        cout << "    [" << slot << "] Пусто" << endl;
                    }
                    else {
                        cout << "    [" << slot << "] " << heroes[i].equipment[slot].name << endl;
                    }
                }
            }

            int usedSlots = 0;
            int totalValue = 0;
            int totalWeight = 0;
            for (int i = 0; i < 16; ++i) {
                if (!inventory[i].name.empty()) {
                    usedSlots++;
                    totalValue += inventory[i].value;
                    totalWeight += inventory[i].weight;
                }
            }

            cout << "\nСостояние инвентаря:" << endl;
            cout << "Занято слотов: " << usedSlots << "/16" << endl;
            cout << "Суммарная стоимость: " << totalValue << endl;
            cout << "Суммарный вес: " << totalWeight << endl;

            cout << "\nСписок врагов:" << endl;
            if (enemyCount == 0) {
                cout << "Нет врагов." << endl;
            }
            else {
                for (int i = 0; i < enemyCount; ++i) {
                    cout << "[" << i << "] " << enemies[i].name
                        << " | HP=" << enemies[i].hp
                        << " | damage=" << enemies[i].damage << endl;
                }
            }
            break;
        }

        case 0:
            running = false;
            break;

        default:
            cout << "Некорректный пункт." << endl;
        }
    }

    // Освобождение памяти перед выходом
    for (int i = 0; i < static_cast<int>(heroes.size()); ++i) {
        delete[] heroes[i].equipment;
    }
    delete[] enemies;

    cout << "Программа завершена." << endl;
    return 0;
}
