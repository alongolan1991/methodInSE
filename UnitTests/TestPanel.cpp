#include "stdafx.h"
#include "CppUnitTest.h"
#include "ScreenMock.h"
#include "LabelMock.h"
#include "../Components/Panel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/*
 * Unit test class for Panel, a composite component responsible of drawing it's decendants. 
 * In these tests the decandants are being mocked and so is the screen they are drawn to.
 */

namespace UnitTests
{		
	TEST_CLASS(TestPanel)
	{
		Panel *p;
		UIComponent *label;
		std::string labelText;

	public:

		TEST_METHOD(Constructor)
		{
			p = new Panel(0, 0, CCTRL.getConsoleSize().X - 1, CCTRL.getConsoleSize().Y - 1, 
				Double, White, Black, NULL);
			Assert::IsNotNull(p);
			Assert::AreEqual(static_cast<int>(Black), static_cast<int>(p->getBackgroundColor()));
			Assert::AreEqual(static_cast<int>(White), static_cast<int>(p->getTextColor()));
			Assert::AreEqual(static_cast<int>(Double), static_cast<int>(p->getBorderType()));
			Assert::AreEqual(0, static_cast<int>(p->getPosition().X));
			Assert::AreEqual(0, static_cast<int>(p->getPosition().Y));
			Assert::AreEqual(CCTRL.getConsoleSize().X - 1, p->getWidth());
			Assert::AreEqual(CCTRL.getConsoleSize().Y - 1, p->getHeight());
		}

		TEST_METHOD(AddComponenet)
		{
			Constructor();
			label = new LabelMock(labelText, 5, 15, 15, 7, Double, Blue, Green);
			p->addComponent(label);
			Assert::IsTrue(true); // if we got here, exception was not raised..
		}

		TEST_METHOD(Draw)
		{
			AddComponenet();
			SCREEN; // init screen mock
			p->draw();
			int textLen = labelText.length();
			int yIndex = label->getYPosition() + 1; // labels' texts are written at y + 1
			int startOfText = label->getXPosition() + 1 + label->getWidth()/2 - textLen/2;

			Assert::AreEqual(labelText,
				SCREEN.getScreen()[yIndex].str().substr(startOfText, textLen));
		}

		TEST_METHOD(SetHeight) {
			Constructor();
			p->setHeight(15);
			p->setHeight(-1); // should not work
			Assert::AreEqual(15, p->getHeight());
		}

		TEST_METHOD(SetWidth) {
			Constructor();
			p->setWidth(15);
			p->setWidth(-1); // should not work
			Assert::AreEqual(15, p->getWidth());
		}

		TEST_METHOD(SetPosition) {
			Constructor();
			p->setPosition(10, 10);
			p->setPosition(-1, -1); // should not work
			Assert::AreEqual(10, static_cast<int>(p->getPosition().X));
			Assert::AreEqual(10, static_cast<int>(p->getPosition().Y));
		}

		TEST_METHOD(GetComponentAt) {
			AddComponenet();
			Assert::IsNotNull(p->getComponentAt(6, 16));
			Assert::IsNull(p->getComponentAt(1, 1));
		}

		TEST_METHOD(RemoveComponent) {
			AddComponenet();
			p->removeComponent(label);
			Assert::IsNull(p->getComponentAt(6, 16));
		}

		TEST_METHOD(RemoveAllComponents) {
			AddComponenet();
			p->removeAll();
			Assert::IsNull(p->getComponentAt(6, 16));

			SCREEN.destroy();
		}
	};
}