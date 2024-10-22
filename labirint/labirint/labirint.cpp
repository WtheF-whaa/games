#include <SDL.h> 
#include <iostream> 
#include <vector> 

using namespace std;

// Структура для стен лабиринта 
struct Wall {
    SDL_Rect rect;
};

// Функция для отрисовки лабиринта 
void drawMaze(SDL_Renderer* renderer, const vector<Wall>& walls) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет для стен 
    for (const auto& wall : walls) {
        SDL_RenderFillRect(renderer, &wall.rect);
    }
}

int main(int argc, char* argv[]) {
    // Инициализация SDL 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Создаем окно 
    SDL_Window* window = SDL_CreateWindow("Простая игра на SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Создаем рендер 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Ошибка создания рендера: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Загрузка изображения персонажа 
    SDL_Surface* tempSurface = SDL_LoadBMP("charactergirl.bmp");
    if (!tempSurface) {
        cout << "Ошибка загрузки изображения: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* characterTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    SDL_Rect characterRect = { 20, 20, 64, 64 }; // Положение и размер персонажа 
    const int speed = 1; // Скорость перемещения 

    // Определяем границы окна 
    int windowWidth = 1200;
    int windowHeight = 800;

    // Создаем лабиринт (стены) 
    vector<Wall> walls = {
            {{0, 0, 5, 800}}, // граница левая
            {{1195, 0, 5, 800}}, // граница правая
            {{150, 0, 1200, 5}}, // граница верхняя
            {{0, 795, 150, 5}}, //граница нижняя
            {{300, 795, 1000, 5}},
            {{150, 0, 10, 120}}, //вертикальная 1
            {{700, 0, 10, 250}}, // вертикальная 2
            {{600, 480, 10, 150}}, // вертикальная 3
            {{150, 480, 10, 150}}, // вертикальная 4
            {{300, 620, 10, 200}}, // вертикальная 5
            {{850, 130, 10, 230}}, // вертикальная 6
            {{1020, 130, 10, 120}}, // вертикальная 7
            {{760, 620, 10, 1000}}, // вертикальная 8
            {{0, 240, 550, 10}}, //горизонтальная 1
            {{150, 120, 400, 10}}, //горизонтальная 2
            {{150, 350, 1050, 10}}, // горизонтальная 3
            {{0, 480, 450, 10}}, // горизонтальная 4
            {{600, 480, 450, 10}}, // горизонтальная 5
            {{300, 620, 300, 10}}, // горизонтальная 6
            {{850, 130, 170, 10}}, // горизонтальная 7
            {{760, 620, 290, 10}}, // горизонтальная 8

    };

    // Основной цикл игры 
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        // Обрабатываем события 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Получение состояния клавиш 
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        // Временные переменные для нового положения персонажа 
        SDL_Rect newCharacterRect = characterRect;

        // Обработка нажатий клавиш WASD 
        if (keystate[SDL_SCANCODE_W]) {
            newCharacterRect.y -= speed;
        }
        if (keystate[SDL_SCANCODE_S]) {
            newCharacterRect.y += speed;
        }
        if (keystate[SDL_SCANCODE_A]) {
            newCharacterRect.x -= speed;
        }
        if (keystate[SDL_SCANCODE_D]) {
            newCharacterRect.x += speed;
        }

        // Проверка столкновений со стенами 
        bool collision = false;
        for (const auto& wall : walls) {
            if (SDL_HasIntersection(&newCharacterRect, &wall.rect)) {
                collision = true;
                break;
            }
        }

        // Перемещение персонажа, если нет столкновений 
        if (!collision) {
            characterRect = newCharacterRect;
        }

        // Очищаем экран 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Черный фон 
        SDL_RenderClear(renderer);

        // Отрисовка лабиринта 
        drawMaze(renderer, walls);

        // Отрисовка персонажа 
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);

        // Обновляем экран 
        SDL_RenderPresent(renderer);
    }

    // Очищаем ресурсы 
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};