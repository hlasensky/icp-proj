/*! \mainpage ICP Project Index Page
 *
 * \section intro_sec Introduction
 *
 *  Tento projekt vytváří hru v prostředí Qt Creatoru, ve které se odehrává simulace s RC roboty, autonomními roboty a překážkami.
 *
 *	Typy robotů a jejich chování
 *
 *  RC roboti: Jsou to roboti na ovládání, které hráč může aktivně ovládat. Hra umožňuje přepínat mezi různými RC roboty, přičemž aktivní robot je možné ovládat, zatímco ostatní RC roboti jsou neaktivní a stojí na místě.
 *  Autonomní roboti: Tito roboti se pohybují samostatně, bez zásahu hráče. Hra umožňuje měnit různé atributy autonomních robotů, jako je vzdálenost detekce překážek nebo jiných robotů, směr otáčení po detekci překážky nebo jiného robota, a také velikost úhlu, o který se robot otočí.
 *
 *	Překážky a manipulace s nimi
 *
 *  Překážky: Překážky jsou statické objekty na mapě, které brání pohybu robotů. Hra umožňuje umístit překážky na libovolné místo na mapě a měnit jejich šířku a výšku. Překážky lze umístit i na sebe navzájem. Jakmile je překážka umístěna na robotovi, znehybní ho  a nemůže se pohybovat.
 *
 * \section implement_sec Implementation
 *
 * \subsection structure 1: Main structure
 *	- Aplikace začíná vytvořením instance třídy QApplication, následuje vytvoření hlavního okna MainWindow, které se zobrazí pomocí metody show(), a nakonec spuštění aplikační smyčky pomocí a.exec(), která udržuje aplikaci aktivní a reaguje na události.
 *	- Konstruktor třídy MainWindow inicializuje UI, nastavuje pohled (view) jako viewport pro graphicsView, připravuje stavy hry a časovač pro periodické *aktualizace scény.
 *	- Uživatel může přidávat roboty a překážky do scény pomocí tlačítek v uživatelském rozhraní. Objekty jsou přidávány pomocí metod setRumbas, setRumbasRC a setObstacles. Po přidání objektů je možné je upravovat kliknutím levým tlačítkem myši.
 *	- RC robot a autonomní robot jsou vytvářeny pomocí konstruktorů tříd RumbaRC a Rumba s nastavením atributů. RC robotovi je také vytvořen směrový indikátor pro lepší orientaci.
 *	- Hlavní smyčka programu: Metoda updateScene() zajišťuje průchod všemi objekty ve scéně a průběžně kontroluje kolize. Pokud kolize nastane, krok se vrátí *pět. Dále se nastavuje aktivní prvek na základě kliknutí myši a barevné označení objektu na červenou.
 *
 * \section usage_sec Usage
 *  Pro spuštění aplikace použijte příkaz make run. Po zobrazení okna budete mít přístup k ovládacímu menu na levé straně pro přidání objektů do scény. Objekty se přidávají pomocí tlačítka s šipkou nahoru vedle pole "Přidat/Odebrat", šipka dolů odebere poslední přidaný objekt dané kategorie. Při přidávání autonomního robota můžete nastavit vzdálenost detekce pomocí posuvníku, úhel rotace pomocí spineru a směr otočení pomocí kulatého tlačítka.
 *  Ovládání RC robota je možné pomocí kláves W, A, S, D nebo tlačítek zobrazených na levé straně, na která můžete kliknout levým tlačítkem myši. Překážky lze upravovat také pomocí posuvníku.
 *   Na pravé straně obrazovky jsou tlačítka pro následující akce:
 *	- Quit: Ukončí hru.
 *	- Pause: Pozastaví hru.
 *	- Save: Uloží aktuální stav hry.
 *	- Load: Načte poslední uložený stav hry.
 *	- New Game: Vymaže aktuální hru a zobrazí prázdné hrací pole.
 *
 *  V horní liště jsou dvě nabídky:
 *	- Game: Obsahuje tlačítka pro hru (jako na pravé straně).
 *	- Examples: Obsahuje tlačítka pro zobrazení ukázkových her
 *
 * \section test_sec Testing
 * 
 * 
 * \section flaw_sec Known flaws
 *  \bug When holding Obstacle with drag you can place them on Rumba and RumbaRC objects and they stay stack under the Object.
 * 
 *  \bug When specifically pressed menu buttons it is possible that game will not show until Resume button is pressed. 
 *  Specifically when Pause -> New Game -> Load Game or Examples are pressed in this order. After Resume button click the game shows and play normaly.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 */