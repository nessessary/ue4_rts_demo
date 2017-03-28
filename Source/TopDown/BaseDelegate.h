#ifndef _BASEDELEGATE_H_
#define _BASEDELEGATE_H_

class BaseDelegate {
public:
	BaseDelegate(void);
	~BaseDelegate(void);
};

// UCLASS can't use interface
namespace RTS {
	// select , unselect, move, attack
	class IObject {
	public:
		virtual void SelectMe(bool is_select) = 0;
		virtual bool IsSelectMe() = 0;
	};
	class IPawn {
	public:
		virtual void MoveTo() = 0;
		virtual void Attack() = 0;
		virtual void Jump() = 0;
		virtual void Stop() = 0;
	};
	class IBuild {
	public:
		virtual void SetRailyPoint() = 0;
		virtual void Spawn() = 0;
	};
};
#endif	// _BASEDELEGATE_H_
