#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Browser");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font file." << std::endl;
        return 1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);

    sf::Http http("http://www.example.com");

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Get);
    request.setUri("/");

    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Ok) {
        std::string content = response.getBody();
        text.setString(content);
    } else {
        text.setString("Failed to load page.");
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
