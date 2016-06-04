//
//      using code from this tutorial:
//      https://veendeta.wordpress.com/2012/02/16/tutorial-getting-started-with-box2d/

// STD
#include <iostream>

// SFML GRAPHICS
#include <SFML/Graphics.hpp>

// BOX2D PHYSICS
#include <Box2D/Box2D.h>

//
//  PROTOTYPE FUNCTIONS; defined below, to be encapsulated correctly later in your engine

// prototypes for ground, box, and circle
void CreateGround(b2World& World, float x, float y);
void CreateBox(b2World& World, float x, float y);
void CreateCircle(b2World& World, float x, float y);

// set scale (pixels per Box2D "meter"
static const float SCALE = 30.0f;

// set iteration quantities for Box2D simulation
int32 VELOCITY_ITERATIONS = 10;
int32 POSITION_ITERATIONS = 5;

//
//      BASIC SFML AND BOX2D INTERGRATION IN ONE PLATE OF SPAGHETTI

int main()
{
    //
    //      SFML THINGS

    // sfml window tweaks
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;

    // open sfml window
    sf::RenderWindow Window(sf::VideoMode(1280/2, 720/2), "Quick SFML + Box2D Example", sf::Style::Close, settings);
    Window.setFramerateLimit(60);

    // camera position top pan
    float camera_x = 0, camera_y = 0 + 6.5;

    // camera zoom
    float camera_zoom = 1.0f;

    // texture declaration
    sf::Texture BackgroundTexture;
    sf::Texture GroundTexture;
    sf::Texture BoxTexture;
    sf::Texture CircleTexture;

    // load textures
    GroundTexture.loadFromFile("ground.png");
    BoxTexture.loadFromFile("box.png");
    CircleTexture.loadFromFile("circle.png");

    //
    //      BOX2D THINGS

    // prepare box2d world
    b2Vec2 Gravity(0.0f, 9.8f);
    b2World World(Gravity, true);

    // add ground
    CreateGround(World, 340.0f, 500.0f);

    //
    //      MAIN LOOP

    while(Window.isOpen())
    {

        // poll window events
        sf::Event event;
        while(Window.pollEvent(event))
        {

        // mouse pos
        float m_x = event.mouseButton.x * 1.0f;
        float m_y = event.mouseButton.y * 1.0f;

        // translate it to a point in Box2D based on camera
        m_x /= camera_zoom;
        m_y /= camera_zoom;
        m_x += (camera_x*SCALE);
        m_y += (camera_y*SCALE);

            switch(event.type)
            {
                case sf::Event::Closed:

                    Window.close();
                    break;

                case sf::Event::MouseButtonPressed:

                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        CreateBox(World, m_x, m_y);
                    }
                    else if(event.mouseButton.button == sf::Mouse::Right)
                    {
                        CreateCircle(World, m_x, m_y);
                    }

                    break;

                case sf::Event::KeyPressed:

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        // close window
                        Window.close();
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    {
                        camera_y -= 1.0f;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    {
                        camera_y += 1.0f;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    {
                        camera_x -= 1.0f;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        camera_x += 1.0f;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        camera_zoom += 0.10f;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        camera_zoom -= 0.10f;
                    }

                    break;

            }

        }

        //      simulate box2d world
        World.Step(1/60.0f, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        //      draw in sfml
        Window.clear();

        //      iterate through objects and draw correctly for type
        for(b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator -> GetNext())
        {

            // current sprite for iterator
            sf::Sprite CurrentSprite;

            // set position
            CurrentSprite.setPosition((SCALE * camera_zoom) * (BodyIterator -> GetPosition().x - camera_x),
                                      (SCALE * camera_zoom) * (BodyIterator -> GetPosition().y - camera_y));

            // apply scale
            CurrentSprite.setScale(camera_zoom, camera_zoom);

            // set angle, convert b2d radians to degrees for sfml
            CurrentSprite.setRotation((BodyIterator -> GetAngle()) * (180 / b2_pi));



            if (BodyIterator -> GetType() == b2_staticBody)
            {

                // set for static bodies (grounds)
                CurrentSprite.setTexture(GroundTexture);
                CurrentSprite.setOrigin(240.0f, 16.0f);

                // draw current sprite
                Window.draw(CurrentSprite);

            }
            else if(BodyIterator -> GetFixtureList() -> GetType() == b2Shape::e_polygon)
            {

                // set for polygons (squares)
                CurrentSprite.setTexture(BoxTexture);
                CurrentSprite.setOrigin(16.0f, 16.0f);

                // draw current sprite
                Window.draw(CurrentSprite);

            }
            else if(BodyIterator -> GetFixtureList() -> GetType() == b2Shape::e_circle)
            {

                // set for circles (balls)
                CurrentSprite.setTexture(CircleTexture);
                CurrentSprite.setOrigin(16.0f, 16.0f);

                // draw current sprite
                Window.draw(CurrentSprite);

            }

        }

        //
        //      display window contents

        Window.display();

    }

    // exit
    return 0;

}

//
//      BELOW: function examples on how to properly create a Box2D body

// ----------------------------------------------------------------------------------------------

//
//      CREATE GROUND

void CreateGround(b2World& World, float x, float y)
{

    // prototype body definintion
    b2BodyDef PROTO_BODY;
    // set parameters; 30 pixels per box2d "meter" (m = 1.0f)
    PROTO_BODY.position = b2Vec2(x/SCALE, y/SCALE);
    PROTO_BODY.type = b2_staticBody;

    // point to body created by b2world
    b2Body* Body = World.CreateBody(&PROTO_BODY);

    // set shape for body
    b2PolygonShape PROTO_SHAPE;
    // creates a box shape; divide your desired w/h by 2
    PROTO_SHAPE.SetAsBox((480.0f/2)/SCALE, (32.0f/2)/SCALE);

    // set fixture definition
    b2FixtureDef PROTO_FIXTURE;
    PROTO_FIXTURE.density = 0.0f;
    PROTO_FIXTURE.shape = &PROTO_SHAPE;

    // apply the fixture prototype to the body
    Body -> CreateFixture(&PROTO_FIXTURE);

}

//
//      CREATE BOX

void CreateBox(b2World& World, float x, float y)
{

    // prototype body definintion
    b2BodyDef PROTO_BODY;
    PROTO_BODY.position = b2Vec2(x/SCALE, y/SCALE);
    PROTO_BODY.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&PROTO_BODY);

    // prototype shape
    b2PolygonShape PROTO_SHAPE;
    PROTO_SHAPE.SetAsBox((32.0f/2)/(SCALE), (32.0f/2)/SCALE);

    // prototype fixture
    b2FixtureDef PROTO_FIXTURE;
    PROTO_FIXTURE.density = 5.0f;
    PROTO_FIXTURE.friction = 5.0f;
    PROTO_FIXTURE.shape = &PROTO_SHAPE;

    // apply the fixture prototype to the body
    Body -> CreateFixture(&PROTO_FIXTURE);

}

//
//      CREATE CIRCLE

void CreateCircle(b2World& World, float x, float y)
{

    // prototype body definintion
    b2BodyDef PROTO_BODY;
    PROTO_BODY.position = b2Vec2(x/SCALE, y/SCALE);
    PROTO_BODY.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&PROTO_BODY);

    // prototype shape
    b2CircleShape PROTO_SHAPE;

    // position, relative to body position
    PROTO_SHAPE.m_p.SetZero();

    // set radius
    PROTO_SHAPE.m_radius = 16/SCALE;

    // prototype fixture
    b2FixtureDef PROTO_FIXTURE;
    PROTO_FIXTURE.density = 5.0f;
    PROTO_FIXTURE.friction = 5.0f;
    PROTO_FIXTURE.shape = &PROTO_SHAPE;

    // apply the fixture prototype to the body
    Body -> CreateFixture(&PROTO_FIXTURE);

}


