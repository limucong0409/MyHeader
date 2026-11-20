namespace MySTL
{
    template<typename T> class zip
    {
    private:
        const int DEFAULT_SIZE=1<<20;
        int HEAD_SIZE,NODE_SIZE,cnt;
        int *he,*ne;
        T *val;
    public:
        zip(int x=0,int y=0)
        {
            !x?x=DEFAULT_SIZE-1:0;
            !y?y=x:0;
            HEAD_SIZE=x;
            NODE_SIZE=y;
            x++,y++;
            cnt=0;
            he=new int[x]{};
            ne=new int[y]{};
            val=new T[y]{};
        }
        void push_back(int x,T y)
        {
            ne[++cnt]=he[x];
            he[x]=cnt;
            val[cnt]=y;
        }
        bool empty(int x){return !he[x];}
        void clear(int x){he[x]=0;}
        class Iterator
        {
        private:
            const zip* at;
            int idx;
        public:
            Iterator(const zip* z,int i):at(z),idx(i){}
            T operator*()const{return (idx?at->val[idx]:T());}
            Iterator& operator++()
            {
                if(idx){idx=at->ne[idx];}
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator tmp=*this;
                ++(*this);
                return tmp;
            }
            bool operator!=(const Iterator& other)const{return idx!=other.idx;}
            bool operator==(const Iterator& other)const{return idx==other.idx;}
        };
        Iterator begin(int u) const{return Iterator(this,he[u]);}
        Iterator end(int u) const{return Iterator(this,0);}
        class Range
        {
        private:
            zip* at;
            int hed;
        public:
            Range(zip* z,int x):at(z),hed(x){}
            Iterator begin()const{return Iterator(at,at->he[hed]);}
            Iterator end()const{return Iterator(at,0);}
            void push_back(T x){at->pushback(hed,x);}
            bool empty(){return at->empty(hed);}
            void clear(){return at->clear(hed);}
        };
        Range nodes(int u){return Range(this,u);}
        Range operator[](int x){return nodes(x);}
        ~zip()
        {
            delete[] he;
            delete[] ne;
            delete[] val;
        }
    };
}