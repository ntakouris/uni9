#include <iostream>
using namespace std;
class Kouti
{
	private:
		double mikos = 2.0;
		double platos;
		double ipsos;
	public:
		void setMikos( double mik )
		{ 
			mikos = mik;
		}
		void setPlatos( double pla )
		{ 
			platos = pla;
		}
		double getPlatos(void){
			 return platos;
		}
		double getMikos (void)
		{
		 return mikos;
		}
		
		virtual void setIpsos( double ips );
		virtual double getIpsos( void );

};


double Kouti::getIpsos( void )
{
	return ipsos;
}

void Kouti::setIpsos ( double ips )
{
	ipsos = ips;
}

int main( )
{
	Kouti Small;
	Kouti Big;
	double ogkos = 0.0;
	Small.setPlatos(1.0);
	Small.setIpsos(2.0);
	
	ogkos = Small.getPlatos() * Small.getIpsos() * Small.getMikos();
	cout << "Ogkos gia Kouti Small: " << ogkos << endl;
	Big.setMikos(12.0);
	Big.setPlatos(13.0);
	Big.setIpsos(10.0);
	ogkos = Big.getPlatos() * Big.getIpsos() * Big.getMikos();
	cout << "Ogkos gia Kouti Big: " << ogkos << endl;
	
	 return 0;
}

