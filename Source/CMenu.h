namespace game_framework {

	class CMenu
	{
	public:
		void Initialize();						// 设定菜单初始值
		void Initialize(int num);				// 设定菜单初始值
		void LoadBitmap();						// 加载图片
		void OnShow();							// 显示图片
		void setNumber(int number);
		void setIsShow(bool flag);
		void setMovingUp(bool flag);
		void setMovingDown(bool flag);
		void setIsLeft(bool flag);
		void setIsSpace(bool flag);
		void OnMove();
		int getStatusNum();
		void setFlag(bool Flag);
		int getWay();							//获取第一层或第二层菜单的编码，1为第一层，2位第二层
	protected:
		CMovingBitmap starts;					// 菜单选项1的图片
		CMovingBitmap help;						// 菜单选项2的图片
		CMovingBitmap about;					// 菜单选项3的图片
		CMovingBitmap mouse;					// 光标
		CMovingBitmap about_background;			// 关于的图片
		CMovingBitmap player_background;		 
		bool isMovingUp;						// 是否按了上键
		bool isMovingDown;						// 是否按了下键
		bool isShow;							// 显示
		bool isLeft;							// 按下左键退出第二层菜单
		bool isSpace;							// 按下空格进入第二层菜单
		bool flag;								   
		int x, y;								// 第一层菜单，小炸弹人的坐标
		int way;								// 获取菜单编码 ，1为第一层，2为第二层
	private:
		int number;
	};
}