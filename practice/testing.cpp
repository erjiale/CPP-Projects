#include <iostream>

class Pen {
    public:
        // Constructor
        Pen() { ink_color = "none"; }

        // Getters
        std::string getInkColor() { return ink_color; }

        // Setters
        void setInkColor(std::string color) { ink_color = color; }

    private:
        std::string ink_color;
};

int main()
{
    Pen mypen;
    
    mypen.setInkColor("red");
    std::string color = mypen.getInkColor();
    std::cout << color << std::endl;

    return 0;
}