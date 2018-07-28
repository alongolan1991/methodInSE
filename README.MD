# Windows Console IO Library

#### A Library for C++ developers to easily create GUI components in windows applications.

###### The project was developed during the course: Methods in Software Engineering, instructed by Dr. Resh Amit in Shenkar, College of Engineering & Design


##### Offered components:

- CheckList: Multiple selection text list
- RadioBox: Single selection text list
- ComboBox: Drop down bar you can select a value from
- Label: Text displaying component
- MessageWindow: Prompt message blocking the screen, user must press "OK" or "Cancel" in order to proceed
- NumericBox: Label with + and - buttons controlling the displayed number.
- TextBox: Mutable text box 
- Panel: Composite component which can hold other components together
- Button: Clickable component you can place text on, in order to attach an action to the button, 
	  you must create a class that inherit from the Action class and override the action() method, 
	  create an object from the class and provide it's address to the Button's constructor

###### In order to start, you will need to create the data:

![Creation](/images/Creation.PNG)

###### Then, attach the components to the main panel, attach the panel to the console controller (CCTRL), which responsible to attach events to the interactable components, and finally listen to user events:

![Attach and Listen](/images/Attach_Listen_Destroy.PNG)

###### delete panel will free resources of all components attached to it, CCTRLDSTROY will free resources of the console controller.

###### Final result:
![Result](/images/Result.PNG)


#### Developed by:
- [Yoav Saroya](https://github.com/SaroyaMan)
- [Amit Shmuel](https://github.com/AmitShmuel)
- [Yftah Livny](https://github.com/shachmatos)
- [Edan Hauon](https://github.com/edanhauon)