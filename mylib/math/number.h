#include<cstdio>
namespace MyMath
{
	constexpr double PI=3.1415926535;
	constexpr double PI5=3.14159;
	constexpr double PI20=3.14159265358979323846;
	constexpr double PI30=3.141592653589793238462643383279;
	constexpr double PI40=3.1415926535897932384626433832795028841970;
	constexpr double MOD=998224353;

	template<typename T>
	T swap(T& x,T& y){T z=x;x=y,y=z;} 
	template<typename T>
	T swap(T* x,T* y){T* z=x;x=y,y=z;} 
	template<typename T>
	T max(T x,T y){return (x>y?x:y);}
	template<typename T>
	T min(T x,T y){return (x<y?x:y);}
	
	template<typename T>
	double sin(T x,int y=10)
	{
		double a=x,b=1,c=1,sum=0,num;
		for(int i=1;i<=y;i++)
		{
			num=a/b*c;
			b=b*(2*i)*(2*i+1);
			a=a*x*x;
			c=-c;
			sum+=num;
		}
		return sum;
	}
	template<typename T>
	double cos(T x,int y=10){return sin(x+PI/2,y);}
	
	template<typename Ta,typename Tb>
	Ta quick_pow(Ta x,Tb y,Tb mod=MOD)
	{
		Ta base=x,ans=1;
		Tb time=1;
		while(time<y)
		{
			if(time&y)(ans*=base)%=mod;
			time<<=1;
			x=(x*x)%mod;
		}
		return ans;
	}
	
	template<typename T>
	class complex
	{
	public:
		T real_part,imaginary_part;
		complex(T x=0,T y=0)
		{
			real_part=x,imaginary_part=y;
		}
		complex self(){return complex(real_part,imaginary_part);}
		void update(T x=real(),T y=imag()){real_part=x,imaginary_part=y;};
		void update(complex x){real_part=x.real_part,imaginary_part=x.imaginary_part;}
		complex operator +(complex x){return complex(real_part+x.real_part,real_part+x.imaginary_part);}
		complex operator -(complex x){return complex(real_part-x.real_part,real_part-x.imaginary_part);}
		complex operator *(complex x){return complex(real_part*x.real_part-imaginary_part*x.imaginary_part,real_part*x.imaginary_part+imaginary_part*x.real_part);}
		complex operator /(complex x){return complex((real_part*x.real_part+imaginary_part*x.imaginary_part)/(x.real_part*x.real_part-x.imaginary_part*x.imaginary_part),(imaginary_part*x.real_part-real_part*x.imaginary_part)/(x.real_part*x.real_part-x.imaginary_part*x.imaginary_part));}
		template<typename TI>void operator =(TI x){update(x,0);}
		void operator +=(complex x){update(self()+x);}
		void operator -=(complex x){update(self()-x);}
		void operator *=(complex x){update(self()*x);}
		void operator /=(complex x){update(self()/x);}
		complex conjugate(){return complex(real_part,-imaginary_part);}
		T get_r(){return real_part;}
		T real(){return real_part;}
		T get_i(){return imaginary_part;}
		T imag(){return imaginary_part;}
	};
	
	template<typename T>
	complex<double> get_w(T x){return complex<double>(cos(PI/x),sin(PI/x));}
	
	template<typename T>
	class polynome
	{
	public:
		T* coefficient;
		int wide;
		polynome()
		{
			wide=0; 
		}
		polynome(T a[],int b)
		{
			coefficient=a,wide=b;
			printf("wide in:%d\n",wide);
		}
		void MakeRev(int* tax,int wd)
		{
			int d=wd>>1,p=0;
			tax[p++]=0;
			tax[p++]=d;
			d>>=1;
			for(int i=2;i<=wd;i<<=1,d>>=1)
			{
				for(int p0=0;p0<i;++p0)
					tax[p++]=tax[p0]|d;
			}
		}
		void Fast_Fourier_Transform(polynome<complex<double>>& P,int tax[],int wd)
		{
			for(int i=1;i<wd;i++) 
				if(tax[i]>i)
					swap(P.coefficient[i], P.coefficient[tax[i]]);
			for(int i=2,j=1;i<=wd;j=i,i<<=1)
			{
				complex<double> W(get_w(j)),w(1,0);
				for(int k=0,l=i-1;l<=wd;k+=i,l+=i)
				{
					complex<double> w0(w);
					for(int m=k,n=k+j;m<n;m++)
					{
						complex<double> x=P.coefficient[m]+w0*P.coefficient[m+j],y=P.coefficient[m]-w0*P.coefficient[m+j];
						P.coefficient[m]=x,P.coefficient[m+j]=y;
						w0*=W;
					}
				}
			}
		}
		int get_wd(){return wide;}
		T get(int x){return coefficient[x];}
		polynome operator +(polynome x)
		{
			int max_wide=max(wide,x.wide);
			polynome re(x.coefficient,max_wide);
			for(int i=0;i<=max_wide;i++)
				re.coefficient[i]+=coefficient[i];
			return re;
		}
		polynome operator -(polynome x)
		{
			int max_wide=max(wide,x.wide);
			polynome re(x.coefficient,max_wide);
			for(int i=0;i<=max_wide;i++)
				re.coefficient[i]=coefficient[i]-re.coefficient[i];
			return re;
		}
		polynome operator *(polynome x)
		{
			int new_wide=wide+x.wide;
			int up_wide=1;
			while(up_wide<=new_wide)up_wide<<=1;
			//printf("*1 wide:%d<-%d=%d+%d\n",up_wide,new_wide,wide,x.wide);
			complex<double> cp[up_wide+1]={},cq[up_wide+1]={};
			for(int i=0;i<=wide;i++)
			{
				cp[i]=coefficient[i];
				printf("cp %d %lf+%lfi\n",coefficient[i],cp[i].get_r(),cp[i].get_i());
			}
			for(int i=0;i<=x.wide;i++)
			{
				cq[i]=x.coefficient[i];
				printf("cq %d %lf+%lfi\n",x.coefficient[i],cq[i].get_r(),cq[i].get_i());
			}
			polynome<complex<double>> p(cp,wide);
			polynome<complex<double>> q(cq,x.wide);
			int tax[up_wide+1]={};
			MakeRev(tax,up_wide);
			for(int i=0;i<up_wide;i++)
				//printf("tax:%d %d\n",i,tax[i]);
			Fast_Fourier_Transform(p,tax,up_wide);
			Fast_Fourier_Transform(q,tax,up_wide);
			for(int i=0;i<up_wide;i++)
				p.coefficient[i]*=q.coefficient[i];
			Fast_Fourier_Transform(p,tax,up_wide);
			for(int i=1;i<=up_wide/2;i++)
			{
				printf("bf swap:%lf+%lfi %lf+%lfi\n",p.coefficient[i].get_r(),p.coefficient[i].get_i(),p.coefficient[up_wide-i+1].get_r(),p.coefficient[up_wide-i+1].get_i());
				swap(p.coefficient[i],p.coefficient[up_wide-i+1]);
				printf("bf swap:%lf+%lfi %lf+%lfi\n",p.coefficient[i].get_r(),p.coefficient[i].get_i(),p.coefficient[up_wide-i+1].get_r(),p.coefficient[up_wide-i+1].get_i());
			}
			polynome<T> re;
			re.wide=new_wide;
			re.coefficient=new T[new_wide+1];
			for(int i=0;i<=new_wide;i++)
			{
				re.coefficient[i]=p.coefficient[i].real()/up_wide+0.5;
				printf("turn cre:%d p:%lf\n",re.coefficient[i],p.coefficient[i].real());
			}
			return re;
		}
	};
}
