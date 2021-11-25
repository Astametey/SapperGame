#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

const int window_width = 800;
const int window_height = 640;

const int map_w = 10;
const int map_h = 10;


int map_cell[map_w][map_h];
int map_mine[map_w][map_h];
int randomvalue;
int IntBomb = 5; 
int ReBomb = 0;
sf::Font font;



void LoadFont() {
    if (!font.loadFromFile("TimesNewRoman.ttf"))
    {
        std::cout << "Failed Load TimesNewRoman.ttf";
    }
    else {
        std::cout << "Font TimesNewRoman.ttf loaded";
    }//Create text font
}

class Cell {
public:
    int IntChar;
    sf::RectangleShape CellRect;
    float X, Y;
    bool open = false;
    bool openText = false;
    bool ReMine = false;
    int value = 0;
    sf::Text Ctext;
    Cell() {
        
    };
    Cell(int x, int y) {
        CellRect.setSize(sf::Vector2f(32, 32));
        CellRect.setPosition(x, y);
        CellRect.setFillColor(sf::Color(0, 61, 0));
        CellRect.setOutlineThickness(-4.f);
        CellRect.setOutlineColor(sf::Color(0, 23, 0));
        X = x; Y = y;

        Ctext.setFont(font);
        Ctext.setPosition(x + 8, y + 4);
        Ctext.setCharacterSize(20);

        
    };

    bool collision(float x1, float y1) {
        if (x1 > X && x1 < X + 32 &&
            y1 > Y && y1 < Y + 32) {
            return true;
        }
        else
        {
            return false;
        }
    }

    void update(float mx, float my) {
        if (collision(mx, my) && !open && !ReMine) {
            CellRect.setFillColor(sf::Color(0, 58, 0));//color rectangle button
        }
        else {
            CellRect.setFillColor(sf::Color(0, 158, 79));//color rectangle button
        }//ButtonHard select

        if (IntChar == 1 && open && value != 1) {
            CellRect.setFillColor(sf::Color(165, 35, 0));
        }

        if (open) {
            if (IntChar == 0) {
                CellRect.setFillColor(sf::Color(0, 161, 201));//color rectangle button
            }
        }

        if (ReMine && !open) {
            CellRect.setFillColor(sf::Color(219, 111, 0));//color rectangle button
        }

        std::ostringstream valuestr;
        valuestr << value;
        Ctext.setString(valuestr.str());

    }
};

class CreateButton {
public:
    sf::Text text;
    sf::RectangleShape Button;//rectangle button
    float W, H, X, Y;
    CreateButton(float x, float y, float w, float h, std::string textstr) {
        Button.setSize(sf::Vector2f(w, h));
        Button.setPosition(x, y);
        Button.setFillColor(sf::Color(0, 158, 79));
        Button.setOutlineThickness(-4.f);
        Button.setOutlineColor(sf::Color(0, 23, 0));

        

        text.setFont(font);
        text.setPosition(x + 8, y + 4);
        text.setString(textstr);
        text.setCharacterSize(20);
        W = w; H = h; X = x; Y = y;
    };

    bool collision(float x1, float y1) {
        if (x1 > X && x1 < X + W &&
            y1 > Y && y1 < Y + H) {
            return true;
        }
        else
        {
            return false;
        }
    }

    void update(float mx, float my) {
        if (collision(mx, my)) {
            Button.setFillColor(sf::Color(0, 58, 0));//color rectangle button
        }
        else {
            Button.setFillColor(sf::Color(0, 158, 79));//color rectangle button
        }//ButtonHard select
    }
    
};

void Map() {
    for (int w = 0; w < map_w; w++) {
        for (int h = 0; h < map_h; h++) {
            map_cell[w][h] = 0;
            map_mine[w][h] = 0;
        }
    }//Clear map
};

void MapMine() {
    for (int i = 0; i < IntBomb; i++) {
        int x = rand() % map_w;
        int y = rand() % map_h;
        map_mine[x][y] = 1;
    };

};//Create mine

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    
    LoadFont();
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Sapper Game");

    sf::Vector2i MousePosWindow = sf::Mouse::getPosition(window);//забираем коорд курсора
    sf::Vector2f MousePos = window.mapPixelToCoords(MousePosWindow);//переводим их в игровые (уходим от коорд окна)
    //ReloadMap();//init map;
    
    
    
    
    Cell Map_Cell[map_w][map_h];

    for (int i = 0; i < map_w; i++) {
        for (int j = 0; j < map_h; j++) {
            Map_Cell[i][j] = Cell(16 + 32 * i,128 + 32 * j);
            Map_Cell[i][j].IntChar = map_mine[i][j];
        };
    };

    Map();
    MapMine();

    for (int i = 0; i < map_w; i++) {
        for (int j = 0; j < map_h; j++) {
            Map_Cell[i][j] = Cell(16 + 32 * i, 128 + 32 * j);
            Map_Cell[i][j].IntChar = map_mine[i][j];
        };
    };

    

    //init resurs
    CreateButton ButtonSelectHard(0, 0, 128, 32, "Reload");

    

    // window 
    while (window.isOpen())
    {
        // Проверить все события окна, которые были запущены с момента последней итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // событие "закрыть запрошенное": закрываем окно
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        MousePosWindow = sf::Mouse::getPosition(window);//забираем коорд курсора
        MousePos = window.mapPixelToCoords(MousePosWindow);//переводим их в игровые (уходим от коорд окна)
        //Create button
        

        
        //step
        ButtonSelectHard.update(MousePos.x, MousePos.y);
        if (ButtonSelectHard.collision(MousePos.x, MousePos.y)) {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    Map();
                    MapMine();

                    for (int i = 0; i < map_w; i++) {
                        for (int j = 0; j < map_h; j++) {
                            Map_Cell[i][j] = Cell(16 + 32 * i, 128 + 32 * j);
                            Map_Cell[i][j].IntChar = map_mine[i][j];
                        };
                    };
                };
            };
        };


        for (int w = 0; w < map_w; w++) {
            for (int h = 0; h < map_h; h++) {

                Map_Cell[w][h].update(MousePos.x, MousePos.y);



                if (Map_Cell[w][h].collision(MousePos.x, MousePos.y)) {
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        

                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            Map_Cell[w][h].open = true;
                            Map_Cell[w][h].openText = true;
                        }

                        if (Map_Cell[w][h].open && Map_Cell[w][h].IntChar == 0) {
                            int cnt = 0;
                            for (int i = w - 1; i <= w + 1 && i < map_w; i++) {
                                for (int j = h - 1; j <= h + 1 && j < map_h; j++) {
                                    if (i >= 0 && j >= 0) {
                                        cnt += map_mine[i][j];
                                    };
                                };
                            };
                            Map_Cell[w][h].IntChar = cnt;
                            Map_Cell[w][h].value = Map_Cell[w][h].IntChar;
                        };

                        if (event.mouseButton.button == sf::Mouse::Right)
                        {
                            if (!Map_Cell[w][h].ReMine) {
                                Map_Cell[w][h].ReMine = true;
                            }
                            else {
                                Map_Cell[w][h].ReMine = false;
                            }

                            if (map_mine[w][h] == 1 && Map_Cell[w][h].ReMine) {
                                ReBomb += 1;
                            }

                            if (map_mine[w][h] == 1 && !Map_Cell[w][h].ReMine) {
                                ReBomb -= 1;
                            }


                            Sleep(200);

                            
                        };
                        

                    }
                }


                if ((map_mine[w][h] == 1 && Map_Cell[w][h].open) || ReBomb == IntBomb) {

                    ReBomb = 0;
                    Map();
                    MapMine();

                    for (int i = 0; i < map_w; i++) {
                        for (int j = 0; j < map_h; j++) {
                            Map_Cell[i][j] = Cell(16 + 32 * i, 128 + 32 * j);
                            Map_Cell[i][j].IntChar = map_mine[i][j];
                        };
                    };

                    Sleep(100);
                }



            };
        };

        // clear window
        window.clear(sf::Color(0, 35, 0));

        //Draw Map
        for (int w = 0; w < map_w; w++) {
            for (int h = 0; h < map_h; h++) {
                window.draw(Map_Cell[w][h].CellRect);
                if (Map_Cell[w][h].openText && Map_Cell[w][h].value > 0) {
                    window.draw(Map_Cell[w][h].Ctext);
                }
                

            };
        };
        
        // window.draw (...);
        window.draw(ButtonSelectHard.Button);
        window.draw(ButtonSelectHard.text);
        
        

        // завершаем текущий кадр
        window.display();
    }

    return 0;
}