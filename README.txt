# icp-proj

PROJEKTOVÝ POPIS

   Tento projekt vytváří hru v prostředí Qt Creatoru, ve které se odehrává simulace s RC roboty, autonomními roboty a překážkami.

	Typy robotů a jejich chování

   RC roboti: Jsou to roboti na ovládání, které hráč může aktivně ovládat. Hra umožňuje přepínat mezi různými RC roboty, přičemž aktivní robot je možné ovládat, zatímco ostatní RC roboti jsou neaktivní a stojí na místě.
   Autonomní roboti: Tito roboti se pohybují samostatně, bez zásahu hráče. Hra umožňuje měnit různé atributy autonomních robotů, jako je vzdálenost detekce překážek nebo jiných robotů, směr otáčení po detekci překážky nebo jiného robota, a také velikost úhlu, o který se robot otočí.

	Překážky a manipulace s nimi

   Překážky: Překážky jsou statické objekty na mapě, které brání pohybu robotů. Hra umožňuje umístit překážky na libovolné místo na mapě a měnit jejich šířku a výšku. Překážky lze umístit i na sebe navzájem. Jakmile je překážka umístěna na robotovi, znehybní ho  a nemůže se pohybovat.


HLAVNÍ STRUKTURA
  
- Aplikace začíná vytvořením instance třídy QApplication, následuje vytvoření hlavního okna MainWindow, které se zobrazí pomocí metody show(), a nakonec spuštění aplikační smyčky pomocí a.exec(), která udržuje aplikaci aktivní a reaguje na události.
- Konstruktor třídy MainWindow inicializuje UI, nastavuje pohled (view) jako viewport pro graphicsView, připravuje stavy hry a časovač pro periodické aktualizace scény.
- Uživatel může přidávat roboty a překážky do scény pomocí tlačítek v uživatelském rozhraní. Objekty jsou přidávány pomocí metod setRumbas, setRumbasRC a setObstacles. Po přidání objektů je možné je upravovat kliknutím levým tlačítkem myši.
- RC robot a autonomní robot jsou vytvářeny pomocí konstruktorů tříd RumbaRC a Rumba s nastavením atributů. RC robotovi je také vytvořen směrový indikátor pro lepší orientaci.
- Hlavní smyčka programu: Metoda updateScene() zajišťuje průchod všemi objekty ve scéně a průběžně kontroluje kolize. Pokud kolize nastane, krok se vrátí zpět. Dále se nastavuje aktivní prvek na základě kliknutí myši a barevné označení objektu na červenou.
