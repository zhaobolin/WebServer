namespace zbase
{
		class noncopyable
		{
				protected:
						noncopyable();
						~noncopyable();
				private:
						noncopyable(const noncopyable&);
						const noncopyable& operator=(const noncopyable&);

		};
}
