#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>
#include "bloque.h"
using namespace std;


Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
	
}

void Game::Loop()
{
	while(wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawSprite();
		DrawGame();
		
		wnd->display();
	}
}
void Game::DrawSprite()
{
	Bloquee* _bloque = new Bloquee(phyWorld, wnd);
_bloque->ActualizarPosiciones(controlBody);_bloque->Dibujar();
	
	
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{ 
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	// Dibujamos el cuerpo de control (c�rculo

	// Dibujar texto en la esquina superior izquierda
	sf::Font font;
	if (!font.loadFromFile("font.ttf")) {
		// Si la fuente no se pudo cargar, muestra un mensaje en la consola
		return;
	}
	sf::Text text;
	text.setFont(font);
	text.setString("Arrow Keys");
	text.setCharacterSize(9); // Tama�o del texto
	text.setFillColor(sf::Color::Red);
	text.setPosition(5, 5); // Posici�n en la esquina superior izquierda
	
	// Verificar si la posici�n del texto est� dentro de la ventana
	if (text.getPosition().x < 0 || text.getPosition().y < 0 ||
		text.getPosition().x + text.getLocalBounds().width > wnd->getSize().x ||
		text.getPosition().y + text.getLocalBounds().height > wnd->getSize().y) {
		std::cerr << "La posici�n del texto est� fuera de la ventana" << std::endl;
		return;
	}
	wnd->draw(text);
	
}

void Game::DoEvents()
{
	Event evt;
	while(wnd->pollEvent(evt))
	{
		switch(evt.type)
		{
			case Event::Closed:
				wnd->close();
				break;
			case Event::MouseButtonPressed:
				b2Body *body=Box2DHelper::CreateTriangularDynamicBody(phyWorld,b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
				// Transformamos las coordenadas segun la vista activa
				Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
				break;
		}
	}

	// Movemos el cuerpo
	controlBody->SetAwake(true);
	b2Vec2 force1(-500.0f, 0.0f);
	b2Vec2 force2(500.0f, 0.0f);
	b2Vec2 point(controlBody->GetWorldCenter());
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->ApplyForce(force1, point, true);
	if(Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->ApplyForce(force2, point, true);
	b2Vec2 force3(0, -1000);
	b2Vec2 force4(0, 1000);
	if (Keyboard::isKeyPressed(Keyboard::Up)) {

		controlBody->ApplyForce(force3, point, true);
	}
		
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
        controlBody->ApplyForce(force4, point, true);
	}
		

}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	//MyContactListener* l= new MyContactListener();
	//phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
	

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);
	

	// Creamos un c�rculo que controlaremos con el teclado
	controlBody= Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 10, 0.5, 0.1f, 1);
	controlBody->SetTransform(b2Vec2(25.0f, 95.0f), 0.0f);

	
	
	

}

Game::~Game(void)
{ }