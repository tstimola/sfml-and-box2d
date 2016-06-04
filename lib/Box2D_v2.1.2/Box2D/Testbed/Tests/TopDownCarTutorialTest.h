#ifndef TOPDOWNCARTUTORIALTEST_H_INCLUDED
#define TOPDOWNCARTUTORIALTEST_H_INCLUDED

// include tire
#include "Tire.h"

class TopDownCarTutorialTest : public Test
{

    Tire tire1;

    public:

        TopDownCarTutorialTest()
        {
            // set no gravity
            m_world -> SetGravity(b2Vec2(0, 0));


            // add a tire
            tire1 = Tire(m_world);
        }

        void Step(Settings* settings)
        {

            // run default physics and rendering
            Test::Step(settings);

            // set test text
            m_debugDraw.DrawString(5, m_textLine, "Tutorial Test");
            m_textLine += 15;

        }

        static Test* Create()
        {
            // return this
            return new TopDownCarTutorialTest();
        }

};

#endif // TOPDOWNCARTUTORIALTEST_H_INCLUDED
