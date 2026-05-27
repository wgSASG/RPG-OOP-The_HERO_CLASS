#include <iostream>
#include <string>

// 1. dependencia (uses a)

class Potion {
public:
    void consume() {
        std::cout << "[Accion] Glug glug. Pocion consumida.\n";
    }
};

// 2. Clases base y componentes

class GameObject {
public:
    GameObject() { std::cout << "[Memoria] GameObject base creado.\n"; }
    virtual ~GameObject() { std::cout << "[Memoria] GameObject base destruido.\n"; }
};

class Stats {
public:
    int health = 100;
    int mana = 50;
    //Composicion ya que el hero cuando muere pierde sus estadisticas
    Stats() { std::cout << "[Memoria] Stats creados.\n"; }
    ~Stats() { std::cout << "[Memoria] Stats destruidos.\n"; }
};

class SoulMark {
public:
    //Composicion ya que el hero cuando muere pierde su SoulMark y cuando muere el heroe el Soulmark "desaparece" con el
    SoulMark() { std::cout << "[Memoria] SoulMark creado.\n"; }
    ~SoulMark() { std::cout << "[Memoria] SoulMark destruido.\n"; }
};

class Guild {
public:
    std::string name;
    //Aqui usamos Agregacion ya que cuando muere el heroe no desaparece el Guild, este queda a pesar de que se muera el heroe
    Guild(std::string n) : name(n) {
        std::cout << "[Memoria] Guild '" << name << "' creado.\n";
    }
    ~Guild() {
        std::cout << "[Memoria] Guild '" << name << "' destruido.\n";
    }
};


// 3. Armas (polimorfismo)

class Weapon {
public:
    std::string name;
    int baseDamage;
    
    Weapon(std::string n, int d) : name(n), baseDamage(d) {}
    
    // Ocupamso un metodo virtual aca, ya que asi podemos usar el "ataque" con varias armas, osea que puedan usar la frase de "Ataque con (el arma)..."
    virtual void attack() {
        std::cout << "[Combate] Ataque con " << name << " haciendo " << baseDamage << " de daño.\n";
    }
    
    virtual ~Weapon() { std::cout << "[Memoria] Arma base destruida.\n"; }
};

class Sword : public Weapon {
public:
    int sharpness;
    
    Sword(std::string n, int d, int s) : Weapon(n, d), sharpness(s) {}
    
    void attack() override {
        std::cout << "[Combate] Rebanado con " << name << " haciendo " << (baseDamage + sharpness) << " de daño!\n";
    }
    
    ~Sword() { std::cout << "[Memoria] Espada destruida.\n"; }
};


// 4. Clase Hero

class Hero : public GameObject {
private:
    // Usamos composicion, ya que es com si vivieran con el heroe, osea si el heroe muere estas igual desapareceran con el.
    Stats stats;
    SoulMark soulMark;
    
    // Aqui en cambio utilizamos agregacion y usamos punteros, ya que como no le pertenecen al heroe, si este muere no se eliminan con el, simplemente se elimina como el acceso que tenia hacia ellos.
    Guild* guild;
    Weapon* weapon;

public:
    Hero() : guild(nullptr), weapon(nullptr) {
        std::cout << "[Memoria] Heroe creado.\n";
    }
    
    ~Hero() override {
        std::cout << "[Memoria] Heroe destruido. (Stats y SoulMark mueren con el (Son eliminadas).\n";
    }

    void joinGuild(Guild* g) { guild = g; }
    void equipWeapon(Weapon* w) { weapon = w; }
    
    void usePotion(Potion* p) {
        if(p) {
            p->consume();
        }
    }

    void performAttack() {
        if(weapon) {
            weapon->attack(); 
        } else {
            std::cout << "[Combate] El heroe ataca con los puños!\n";
        }
    }
};


// 5. Main
// Para ver si al crear el hereo todo esta bien, y tambien ver si, en caso de ser eliminado el heroe todo desaparece.

int main() {
    std::cout << "=== 1. CREANDO ENTORNOS EXTERNOS (AGREGACION) ===\n";
    Guild* myGuild = new Guild("Los Gnomos de Jardin");
    Weapon* mySword = new Sword("Mata-Bis-Abuelitas", 15, 5);
    Potion healthPotion;

    std::cout << "\n=== 2. INVOCANDO AL HEROE ===\n";
    Hero* hero = new Hero();

    std::cout << "\n=== 3. FORMANDO RELACIONES ===\n";
    hero->joinGuild(myGuild);
    hero->equipWeapon(mySword);

    std::cout << "\n=== 4. ACCIONES DEL JUEGO ===\n";
    hero->usePotion(&healthPotion);
    hero->performAttack();

    std::cout << "\n=== 5. EL HEROE MUERE (PRUEBA DE MEMORIA) ===\n";
    // Aqui vborramos al heroe pero sin borrar, al Guild y a las armas.
    delete hero;

    std::cout << "\n=== 6. VERIFICANDO SUPERVIVENCIA ===\n";
    std::cout << "El gremio sigue activo: " << myGuild->name << "\n";
    std::cout << "El arma cayo al suelo pero existe: " << mySword->name << "\n";

    std::cout << "\n=== 7. LIMPIEZA FINAL DEL SERVIDOR ===\n";
    delete myGuild;
    delete mySword;

    return 0;
}
