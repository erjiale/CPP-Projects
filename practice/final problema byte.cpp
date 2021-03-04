
#include<iostream> //Biblioteca donde se encuentra la función cout
#include<string>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<filesystem>
#include<ctime>
#include <array>
#include <vector>
#include <iterator>
#include <cmath>
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;
using namespace std;
      namespace fs=std::filesystem;
unsigned t0,t1;
 double load_time=0;
double store_time=0;
string bmp_extension=".bmp";

bool restricciones(const string &file)
{
    static constexpr size_t HEADER_SIZEE = 54;

    ifstream bmp(file, std::ios::binary);

    array<char, HEADER_SIZEE> header;
    bmp.read(header.data(), header.size());

    auto planes = *reinterpret_cast<uint16_t *>(&header[26]);
    //cout << "planes: " << planes << endl;
    
    auto point = *reinterpret_cast<uint16_t *>(&header[28]);
    //cout << "size of point: " << point << endl;
    
    auto compression = *reinterpret_cast<uint32_t *>(&header[30]);
    //cout << "compression: " << compression << endl;
     planes=1;
     point=24;
     compression=0;
	if(planes!=1 || point!=24 || compression!=0){
        cout<<"El fichero no cumple con las caracteristicas solicitadas\n";
            return false;
    }
  

    return true;
}

//***Inputs*****
//fileName: The name of the file to open 
//***Outputs****
//pixels: A pointer to a byte array. This will contain the pixel data
//width: An int pointer to store the width of the image in pixels
//height: An int pointer to store the height of the image in pixels
//bytesPerPixel: An int pointer to store the number of bytes per pixel that are used in the image
std::byte** ReadImage(const char *fileName,std::byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel)
{
        //Open the file for reading in binary mode
        FILE *imageFile = fopen(fileName, "rb");
        //Read data offset
        int32 dataOffset;
        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
        fread(&dataOffset, 4, 1, imageFile);
        //Read width
        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
        fread(width, 4, 1, imageFile);
        //Read height
        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
        fread(height, 4, 1, imageFile);
        //Read bits per pixel
        int16 bitsPerPixel;
        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
        fread(&bitsPerPixel, 2, 1, imageFile);
        //Allocate a pixel array
        *bytesPerPixel = ((int32)bitsPerPixel) / 8;
        //Rows are stored bottom-up
        //Each row is padded to be a multiple of 4 bytes. 
        //We calculate the padded row size in bytes
        int paddedRowSize = (int)(4 * ceil((float)(*width) / 4.0f))*(*bytesPerPixel);
        //We are not interested in the padded bytes, so we allocate memory just for
        //the pixel data
        int unpaddedRowSize = (*width)*(*bytesPerPixel);
        //Total size of the pixel data in bytes
        int totalSize = unpaddedRowSize*(*height);
        *pixels = (std::byte*)malloc(totalSize);
        //Read the pixel data Row by Row.
        //Data is padded and stored bottom-up
        int i = 0;
        //point to the last row of our pixel array (unpadded)
        std::byte *currentRowPointer = *pixels+((*height-1)*unpaddedRowSize);
        for (i = 0; i < *height; i++)
        {
                //put file cursor in the next row from top to bottom
	        fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
	        //read only unpaddedRowSize bytes (we can ignore the padding bytes)
	        fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
	        //point to the next row (from bottom to top)
	        currentRowPointer -= unpaddedRowSize;
	         
        }

        fclose(imageFile);
        return pixels;

    //     int col = 0;
    //     for(int j = 0; j < size(pixels); j++) {
    //         col++;
    //     }



        

    //     for(int row = 0; row < *height; row++) {
    //         for(int col = 0; col < *width; col++) {
    //             // ........
    //         }
    //     }



    // for(recorra pixels,i<size(pixels),3)
    // array int[]
    //     byte[0] byte[1] byte[2] byte aux=0 =32 bits (int)->im(i,j), i=0,j=0;
        
}

//***Inputs*****
//fileName: The name of the file to save 
//pixels: Pointer to the pixel data array
//width: The width of the image in pixels
//height: The height of the image in pixels
//bytesPerPixel: The number of bytes per pixel that are used in the image
void WriteImage(const char *fileName, std::byte *pixels, int32 width, int32 height,int32 bytesPerPixel)
{
        //Open file in binary mode
        FILE *outputFile = fopen(fileName, "wb");
        //*****HEADER************//
        //write signature
        const char *BM = "BM";
        fwrite(&BM[0], 1, 1, outputFile);
        fwrite(&BM[1], 1, 1, outputFile);
        //Write file size considering padded bytes
        int paddedRowSize = (int)(4 * ceil((float)width/4.0f))*bytesPerPixel;
        int32 fileSize = paddedRowSize*height + HEADER_SIZE + INFO_HEADER_SIZE;
        fwrite(&fileSize, 4, 1, outputFile);
        //Write reserved
        int32 reserved = 0x0000;
        fwrite(&reserved, 4, 1, outputFile);
        //Write data offset
        int32 dataOffset = HEADER_SIZE+INFO_HEADER_SIZE;
        fwrite(&dataOffset, 4, 1, outputFile);

        //*******INFO*HEADER******//
        //Write size
        int32 infoHeaderSize = INFO_HEADER_SIZE;
        fwrite(&infoHeaderSize, 4, 1, outputFile);
        //Write width and height
        fwrite(&width, 4, 1, outputFile);
        fwrite(&height, 4, 1, outputFile);
        //Write planes
        int16 planes = 1; //always 1
        fwrite(&planes, 2, 1, outputFile);
        //write bits per pixel
        int16 bitsPerPixel = bytesPerPixel * 8;
        fwrite(&bitsPerPixel, 2, 1, outputFile);
        //write compression
        int32 compression = NO_COMPRESION;
        fwrite(&compression, 4, 1, outputFile);
        //write image size (in bytes)
        int32 imageSize = width*height*bytesPerPixel;
        fwrite(&imageSize, 4, 1, outputFile);
        //write resolution (in pixels per meter)
        int32 resolutionX = 11811; //300 dpi
        int32 resolutionY = 11811; //300 dpi
        fwrite(&resolutionX, 4, 1, outputFile);
        fwrite(&resolutionY, 4, 1, outputFile);
        //write colors used 
        int32 colorsUsed = MAX_NUMBER_OF_COLORS;
        fwrite(&colorsUsed, 4, 1, outputFile);
        //Write important colors
        int32 importantColors = ALL_COLORS_REQUIRED;
        fwrite(&importantColors, 4, 1, outputFile);
        //write data
        int i = 0;
        int unpaddedRowSize = width*bytesPerPixel;
        for ( i = 0; i < height; i++)
        {
                //start writing from the beginning of last row in the pixel array
                int pixelOffset = ((height - i) - 1)*unpaddedRowSize;
                fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);	
                
        }
        fclose(outputFile);
      
}

int getanchura(const string &file)
{
    static constexpr size_t HEADER_SIZEE = 54;

    ifstream bmp(file, std::ios::binary);

    array<char, HEADER_SIZEE> header;
    bmp.read(header.data(), header.size());


    auto width = *reinterpret_cast<uint32_t *>(&header[18]);



    cout << "width: " << width << endl;

    
    return width;

}
int cabecera(const string &file){
     static constexpr size_t HEADER_SIZEE = 54;

    ifstream bmp(file, std::ios::binary);

    array<char, HEADER_SIZEE> header;
    bmp.read(header.data(), header.size());


    auto cabecera = *reinterpret_cast<uint32_t *>(&header[14]);

    
    return cabecera;
}
int getaltura(const string &file)
{
    static constexpr size_t HEADER_SIZEE = 54;

    ifstream bmp(file, std::ios::binary);

    array<char, HEADER_SIZEE> header;
    bmp.read(header.data(), header.size());


    auto height = *reinterpret_cast<uint32_t *>(&header[22]);
    
    cout << "height: " << height << endl;

    
    return height;

}


/*std::byte** gauss(std::byte** pixels, int width, int height){
       
        int M[5][5]={{1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1};
        int w = 273;
        int sum;
        long p=1/w;
        std::byte** res;
        for (int i=0; i<= (rows - 1); i++){
			sum = 0;
            	for(int s=-2;s<=2;s++){
	              	 for(int t=-2;t<=2;t++){ 
                   
                    aux1=s+3;
                    aux2=t+3;
                    aux3=(aux1*aux2)-1;
                    aux4=j+s+t;
                    
                    //cout<<"aux1:"<<aux1<<"\n";
                     //cout<<"aux2:"<<aux2<<"\n";
                      //cout<<"aux3:"<<aux3<<"\n";
                       //cout<<"aux4:"<<aux4<<"\n";
                        //cout<<"aux5:"<<aux5<<"\n";
                    
                    if(aux4<0 ){
                        res=0;
                       //cout<<"Es 0, borde "<<res<<"\n";
                    }else{
                         //aux6=(aux4*aux5)-1;
                         //cout<<"aux6 "<<aux6<<"\n";
                         //cout<<"aux3 "<<aux3<<"\n";
                        //cout<<"Opero M*a y su resultado es "<<int(M[aux3])<<"con"<<int(a[aux6])<<"\n";
                    res=M[aux3]*a[aux4];
                    //cout<<"SUma parcial No es 0, la suma es"<<res<<"\n";
                    }



                    sum=sum+res;
                 
               
            }
        
       
        }
       cout<<"La suma final es "<<sum<<"\n";
        r_final=sum/273;
        cout<<r_final;
         if (r_final>255) r_final = 255;
		if (r_final<0) r_final = 0;
        r[j]=r_final;
     
       
       
        
        
    
        
          
    
    
        }
       
            cout<<a.size();
         return r;
    }*/
    void imprimir_vector(vector<char> a){
            for(int x=0;x<=a.size()-1;x=x+3){
                    printf("%d %d %d",a[x],a[x+1],a[x+2]);
                    printf("\n");
                    
                   
            }
    }

    

std::byte** sobel( std::byte** pixels, int width, int height){
	std::int **res = pixels;
  	int z=0;
  	int colums = width;
   	int rows= height;
	long sumX,sumY;
	int SUM;
	int s,t;
	int Mx[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
	int My[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
	int w=8;
	long p=1/w;

	for (int y=0; y<= (rows - 1); y++){
		for (int x=0; x<=(colums - 1); x++){
			sumX = 0;
			sumY = 0;
			
			if(y==0 || y==(rows-1))
				SUM = 0;
			else if(x==0 || x ==(colums-1))
				SUM=0;
			
			else{
				for(int s=-1; s<=1; s++){
					for (t=-1; t<=1; t++){
		            			//sumX= (sumX + (int)((pixels + x + s + (y + t)*colums) << Mx[s+1][t+1]));
		           			// sumX= p* (sumX + (pixels[x + s + (y + t)*colums]));
		           			// cout<<(int)pixels[y]<<"\n";
					}
				}
				for(int s=-1; s<=1; s++){
					for (t=-1; t<=1; t++){
		            			//sumX= p* (sumX + (int)((*(pixels + x + s + (y + t)*colums)) * My[s+1][t+1]));
		           			 
					}
				}
				SUM = abs(sumX) + abs(sumY);
			}
			
			if (SUM>255) SUM = 255;
			if (SUM<0) SUM = 0;
			 //*(res + x + y*colums) = (std::byte*)(SUM);
			 
		}
	}

	return res;
}

double copy_time(double a, double b){
        double result=(double(a-b)/CLOCKS_PER_SEC);
        result=result*1000;//milisegundos
        return result;
}

int load(string dir){
    ifstream in(dir);
    if(!in){
        return 0;
    }else{
        return 1;
    }

}
int copy_file(string dir_in,string dir_out){
    
    try{
     t0=clock();   
    fs::copy(dir_in,dir_out);
    t1=clock();
    
    }catch(std::filesystem::__cxx11::filesystem_error){
            
            return 0;
    }
    return 1;
   
}
    

int main(int argc, char *argv[]) {
    std::byte *pixels; //se supone que aquí guarda los píxeles.
    int32 width;
    int32 height;
    int32 bytesPerPixel;
    if(argc!=4){//Si el número de los parámetros de entrada no son correctos
        cout<<"Wrong format :\n";
        cout<<" image-seq operation in_path out_path\n";
        cout<<"  operation: copy, gauss, sobel\n";
    }else if(strcmp(argv[1],"copy")!=0 && strcmp(argv[1],"sobel")!=0 && strcmp(argv[1],"gauss")!=0){
        
        cout<<"Unexpected operation:"<<argv[1]<<"\n";
        cout<<" image-seq operation in_path out_path\n";
        cout<<"  operation: copy, gauss, sobel\n";
    }else{
        
        if(load(argv[2])==0){
            cout<<"Input path: "<<argv[2]<<"\n";
            cout<<"Output path: "<<argv[3]<<"\n";
            cout<<"Input directory ["<<argv[2]<<"]"<<"does not exist\n";
            cout<<" image-seq operation in_path out_path\n";
            cout<<"  operation: copy, gauss, sobel\n";

        }else{
            if(load(argv[3])==0){
                cout<<"Input path: "<<argv[2]<<"\n";
                cout<<"Output path: "<<argv[3]<<"\n";
                cout<<"Output directory ["<<argv[3]<<"]"<<"does not exist\n";
                cout<<" image-seq operation in_path out_path\n";
                cout<<"  operation: copy, gauss, sobel\n";

            }else{
                
                if(strcmp(argv[1],"copy")==0){
                            //copy
                    for(auto& p: fs::directory_iterator(argv[2])){
                        
                        string file=p.path();
                        string extension=p.path().extension();
                        
                     
                        
                        if(extension.compare(bmp_extension)==0){
                        int copy_r=copy_file(file,argv[3]);
                    if(copy_r==0){
                        
                        cout<<"Ya existe el archivo: " <<p.path().filename() <<" en la carpeta destino: "<<argv[3]<<"\n";
                    }else{
                    cout<<"Input path: "<<argv[2]<<"\n";
                    cout<<"Output path: "<<argv[3]<<"\n";
                    cout<<"File: "<<""<<p.path().filename()<<"\n";
                    cout<<" Load time: "<<load_time<<"\n";
                    cout<<" Copy time: "<<copy_time(t1,t0)<<"\n";
                    cout<<" Store time: "<<store_time<<"\n";
                    
                    }
                }else{
                        cout<<p.path().filename()<<" No es tipo .bmp \n";
                }
                    
                }
                    
                }
                else if(strcmp(argv[1],"gauss")==0){
                   
                     for(auto& p: fs::directory_iterator(argv[2])){
                         
                           string file=p.path();
                        string extension=p.path().extension();
                        if(extension.compare(bmp_extension)==0){ 
                          
                        
                            if(restricciones(file)){
                               
                                
                               
                                //std::byte pixels =  
                                //ReadImage("Profe-7.bmp", &pixels, &width, &height,&bytesPerPixel);
                                int anchura = getanchura("Profe-7.bmp");
                                int altura = getaltura("Profe-7.bmp");
                                sobel(ReadImage("Profe-7.bmp", &pixels, &width, &height,&bytesPerPixel), anchura, altura);
                                WriteImage("snake2.bmp", pixels, width, height, bytesPerPixel);
                                free(pixels);
                                return 0;
                               
                                //imprimir_vector(img);
                            }else{
                               
                                ReadImage("Profe-7.bmp", &pixels, &width, &height,&bytesPerPixel);
                                WriteImage("snake2.bmp", pixels, width, height, bytesPerPixel);
                                free(pixels);
                                return 0;
                                                        //ERROR
                                
                           
                               // imprimir_vector(img);

                            }
                        
                        //gauss
                }else{
                        cout<<p.path().filename()<<" No es tipo .bmp \n";
                   
                }
               
                }
        }else{

             //sobel y gauss
        }

        

    /*cout<<"El modo de operacion es: "<<argv[1]<<"\n";
    cout<<"El directorio de entrada es: "<<argv[2]<<"\n";
    cout<<"El directorio de salida es: "<<argv[3]<<"\n";*/

    }

}
    }
}
