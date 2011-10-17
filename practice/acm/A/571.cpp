/*   @JUDGE_ID:   10319NX   571   C++*/
#include<iostream.h>
#include<string.h>
#define ACT 6

int A, B, N;
char *name[ACT] = {"fill A", "fill B", "empty A", "empty B", "pour A B", "pour B A" };

class Statu{
	public:
		Statu();
		void set(int, int);
		int a, b;
};

void Statu::set(int aa, int bb)
{
	a = aa;
	b = bb;
}

Statu::Statu(void)
{
	set(0, 0);
}

class BfsStatu: public Statu{
	public:
		BfsStatu();
		void set(int, int, int, int);
		int father, act;
};

void BfsStatu::set(int aa, int bb, int ff, int ac)
{
	a = aa;
	b = bb;
	father = ff;
	act = ac;
}

BfsStatu::BfsStatu(void)
{
	set(0, 0, 0, 0);
}

class Queue{
	public:
		void init();
		Queue();
		void enq(BfsStatu &);
		BfsStatu *deq();
	protected:
		BfsStatu queue[3000];
		int head, tail;
};

void Queue::init(void)
{
	head = 0;
	tail = 0;
}

Queue::Queue(void)
{
	init();
}

void Queue::enq(BfsStatu &node)
{
	if(tail == 10000)
		return;
	queue[tail] = node;
	tail++;
}

BfsStatu *Queue::deq(void)
{
	if(head == tail)
		return 0;
	head++;
	return &queue[head-1];
}

class  BfsQueue: public Queue{
	public:
		BfsQueue();
		void enq(BfsStatu &);
		int check(Statu &);
		int find(Statu &);
		void output();
		void init();
	private:
		int used[1006][1006];
		void search(int);
};

void BfsQueue::init(void)
{
	memset(used, 0, sizeof(used));
	Queue::init();
}

BfsQueue::BfsQueue(void)
{
	init();
}

void BfsQueue::enq(BfsStatu &node)
{
	used[node.a][node.b] = 1;
	node.father = head-1;
	Queue::enq(node);
}

int BfsQueue::check(Statu &node)
{
	if(used[node.a][node.b])
		return 0;
	return 1;
}

int BfsQueue::find(Statu &node)
{
	if(node.b == N)
		return 1;
	return 0;
}

void BfsQueue::output(void)
{
	search(tail-1);
	cout << "success\n";
}

void BfsQueue::search(int n)
{
	if(n > 0){
		search(queue[n].father);
		cout << name[queue[n].act] << endl;
	}
}

class Act{
	public:
		virtual BfsStatu change(Statu &) = 0;
};

class FAact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu FAact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = A;
	tmp.b = node.b;
	return tmp;
}

class FBact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu FBact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = node.a;
	tmp.b = B;
	return tmp;
}

class EAact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu EAact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = 0;
	tmp.b = node.b;
	return tmp;
}

class EBact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu EBact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = node.a;
	tmp.b = 0;
	return tmp;
}

class PABact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu PABact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = node.a-((B-node.b < node.a) ? B-node.b : node.a);
	tmp.b = node.b+((B-node.b < node.a) ? B-node.b : node.a);
	return tmp;
}

class PBAact: public Act{
	public:
		BfsStatu change(Statu &);
};

BfsStatu PBAact::change(Statu &node)
{
	BfsStatu tmp;

	tmp.a = node.a+((A-node.a < node.b) ? A-node.a : node.b);
	tmp.b = node.b-((A-node.a < node.b) ? A-node.a : node.b);
	return tmp;
}

BfsQueue queue;

void initTurn(Act *turn[])
{
	turn[0] = new FAact;
	turn[1] = new FBact;
	turn[2] = new EAact;
	turn[3] = new EBact;
	turn[4] = new PABact;
	turn[5] = new PBAact;
}
int main(void)
{
	BfsStatu init;
	Act *turn[ACT];
	BfsStatu *pos, tmp;
	int i;

	initTurn(turn);
	while(cin >> A >> B >> N){
		queue.init();
		init.set(0, 0, -1, -1);
		queue.enq(init);
		while((pos=queue.deq()) != 0){
			for(i=0; i<ACT; i++){
				tmp = turn[i]->change(*pos);
				tmp.act = i;
				if(queue.check(tmp))
					queue.enq(tmp);
				if(queue.find(tmp)){
					queue.output();
					goto done;
				}
			}
		}
		done:
			;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */