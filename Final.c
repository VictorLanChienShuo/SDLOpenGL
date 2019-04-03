// Include library

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/////// Define bool//////////// 

/* source from http://stackoverflow.com/questions/1921539/using-boolean-values-in-c */

typedef int bool;  
enum { false, true };

///////////////////////////////////////////Functions/////////////////////////////////////////////////////

void Symbol_analyse (TTF_Font * font, SDL_Color textcolor, SDL_Surface * symbol, Uint32 *symbolpixels, int *Symbol_Block_brightness)
{
	/* discussed and helped by Chun You */
	/* This function is use  to analyse the brightness of every ASCII symbols used in this program */
	
	
	int counterX = 1; /* create a temp counter to counts how many time it runs thought the first loop, it begin 
						with 1, because in the third loop, the counterX can't be 0 */

	int arraycounter = 0; /* create a counter to counts how many time it runs thought the first loop */

	int Symboltempbrightness =0; /* create a variable to store the the temporary brightness */


	/*	those for loops are use to scan thought pixels within each ASCII symbol 
		used which been converted into a bitmap image to represented by a matrix 
		of image-width rows by image-height columns that encode the pixels */
	

		for ( int x = 0; x < (symbol->w); x+=6) /* first for loop is use to scan the pixels from the left uppest pixel
												   to the right uppest pixel by jump 6 pixels each (single ASCII symbols width) */
		{

			for (int By = 0; By < 13; By++)  /* second for loop is use to scan the pixels from the left upper pixel 
											    to the 13 pixel (single ASCII symbols height) downward*/
											  
			{
				for (int Bx = x; Bx < counterX*6; Bx++)/* third for loop is use to scan each pixels from the left upper pixel 
											              and move right 6 pixels (single ASCII symbols width) each */
				
				{


					Uint32 symbolpixel = symbolpixels[By * symbol->w + Bx]; /* access to the ASCII symbols pixels */
					Uint8 r = symbolpixel; /* get red value within the pixel */

					Symboltempbrightness += r; /* add red value from each pixel the loop read thought to the temp 
													brightness */
					
				}

					
			}
			counterX++; /* increase the counter x by 1*/
			Symboltempbrightness = Symboltempbrightness / (13*6); /* divide the temp brightness with the surface area of single ASCII symbols
																	 to find the average brightness*/
			
			Symbol_Block_brightness[arraycounter] = Symboltempbrightness;/* store the average brightness to the Symbol_Block_brightness
																			array */
			arraycounter++; /* increase arraycounter by 1 */
							
					
			}		

}

void Symbol_bubblesort(int *Symbol_Block_brightness,char *symboluse )
{ 
	/* this function is use to sort the integers in the Symbol_Block_brightness array 
	   from lowest to highest, so does the ASCII symbol it links to*/
	    
	int i,j,tmp, tmp2;
	for(i=75;i>0;i--) /* optimise by ignoring sorted data */
	{
		for(j=1;j<i;j++) /* step through unsorted elements */
		{
			if(Symbol_Block_brightness[j-1]>Symbol_Block_brightness[j])
			{ 
				/*if neighbours need switching, swap them */
				
				tmp=Symbol_Block_brightness[j-1];
				Symbol_Block_brightness[j-1]=Symbol_Block_brightness[j];
				Symbol_Block_brightness[j]=tmp;
				
				tmp2=symboluse[j-1];
				symboluse[j-1]=symboluse[j];
				symboluse[j]=tmp2;
			}
		}
	}
 }
 
void Symbol_Stretch (int *Symbol_Block_brightness)
{	
	/* This function is uses for stretch the average brightness form 
	   Symbol_Block_brightness to the maximum value as 225 and minimum value as 0 */
	
	/* source from http://mattmik.com/articles/ascii/ascii.html */

	for (int f = 0; f < 75; f++)
	{
	
	Symbol_Block_brightness[f] = (255 * (Symbol_Block_brightness[f] - 0))
	/ (108 - 0);
	}
}


void Image_analyse (SDL_Surface * image, Uint32 *pixels,int Blocknum, int *Image_Block_brightness)
{
	/* This function is to turn the image import into array scale and divide it into  size of 13 * 6 block, 
	   which is the surface area for single ASCII symbol, and analyse the brightness of them */
	/* discussed and helped by Chun You */
	
	int x = 0;
	int y = 0;
	
	int counterX = 1; /* create a temp counter to count how many time it run thought the second loop for in image analyse, it begin 
						with 1, because in the forth loop, the counterX can't be 0 */
	
	int counterY = 1; /* create a temp counter to count how many time it run thought the first loop for in image analyse, it begin 
						with 1, because in the third loop, the counterX can't be 0 */
	
	int HeightCap = image->h/13;  /* create an integer variable for the accurate result of image heigh divide by single ASCII symbol heigh */ 
	int WidthCap = image->w/6;	/* create an integer variable for the accurate result of image width divide by single ASCII symbol width */ 
	

	/* turn the image to gray scale */
	
	/* source from http://www.programmersranch.com/2014/02/sdl2-converting-image-to-grayscale.html */
 
	for (y = 0; y < image->h; y++)
	 {
		  for (x = 0; x < image->w; x++)
		  {
			/* The >> operator shifts bits to the right, and the & is a bitwise AND operator. 
				Each colour byte is shifted to the last byte and then ANDed with the value 0xFF, 
				which is hexadecimal notation for what would be 255 in decimal, That way, we can 
				extract all three colours individually.*/
				  
			   Uint32 pixel = pixels[y * image->w + x];
			   Uint8 r = pixel >> 16 & 0xFF;
			   Uint8 g = pixel >> 8 & 0xFF;
			   Uint8 b = pixel & 0xFF;
			   Uint8 v = 0.212671f * r
                            + 0.715160f * g
                            + 0.072169f * b; /* average the three colours and set each component to that average */
			   pixel = (0xFF << 24) | (v << 16) | (v << 8) | v; /* left-shift them into position */
			   pixels[y * image->w + x] = pixel;

		   }
	   }
	
/////////////////////////////////////// image analyse ///////////////////////////////////////////////
	
	
	/*	those for loops are use to scan thought pixels within each ASCII symbol 
		used which been converted into a bitmap image to represented by a matrix 
		of image-width rows by image-height columns that encode the pixels */

	int counter = 0; /* create a counter to count how many time it run thought the second loop */
	
		for (y = 0; y < HeightCap*13; y+=13)/* first for loop is use to scan the pixels from the left uppest pixel
												   to the left lowerest pixel by jump 13 pixels each (ASCII symbol height) */
		{
			for (x = 0; x < WidthCap*6; x+=6)/* second for loop is use to scan the pixels from the left uppest pixel
												   to the right uppest pixel by jump 6 pixels each (ASCII symbol width) */
			{
			int tempbrightness =0 ; /* create an variable to store the temporary brightness, and set to 0 */
			
				for (int By = y; By < counterY*13; By++)/*third for loop is use to scan the pixels from the left upper pixel 
															to the 13 pixel (ASCII symbol height) downward*/
				{
					for (int Bx = x; Bx < counterX*6; Bx++)/* forth for loop is use to scan each pixels from the left upper pixel 
																and move by 6 pixels (ASCII symbol width) each to the right */
					{

	
						Uint32 pixel = pixels[y * image->w + x];/* access to the image pixels */
						Uint8 r = pixel; /* get red value within the pixel */
	
						tempbrightness += r;/* add red value from each pixel the loop read thought to the temp 
													brightness */
	
						
						
					}
					
				}
				counterX++; /*increase counterX by 1 */
				tempbrightness = tempbrightness / (13*6); /* divide the temp brightness with the surface area of each character 
																	 to find the average brightness*/  
	
				Image_Block_brightness[counter] = tempbrightness; /* store the tembrightness in to Image_Block_brightness array */
	
				counter++; /*counter increase by 1 */
	
			}
			counterY++; /*counterY increase by 1 */
			counterX = 1; /*counterX return to 1 */
			
		}
}	

void Replace (int *Symbol_Block_brightness,  int *Image_Block_brightness, int Blocknum, char *symboluse,char *Replaced_symboluse) 
{	
	/* this function is use match the both Image_Block_brightness array and Symbol_Block_brightness array, 
	 and create another Replaced_symboluse array to store the symbol which have  closest match brightness 
	 to the Image_Block_brightness */
	/* discussed and helped by Lucy Wright */

	int cmp1; /* create first compare variable */
	int cmp2; /* create second compare variable */
	int index; /* create an index */

	
	for (int b=0; b<Blocknum; b++) /* first for loop is to go thought each single ASCII symbol size block divided by the image */
	{
		for (int s = 0; s < 75 ;s++) /* second for loop is to go thought each ASCII symbol used */
		{
			if (Image_Block_brightness[b] == Symbol_Block_brightness[s]) /* if the Image_Block_brightness is equal to Symbol_Block_brightness */
			{	
				Replaced_symboluse[b] = symboluse[s]; /* then the array Replaced_symboluse will stores the symbol */ 
				
			}
			if (Image_Block_brightness[b] > Symbol_Block_brightness[s]) /*if the Image_Block_brightness is bigger then the Symbol_Block_brightness*/
			{	
				cmp2 = Image_Block_brightness[b] - Symbol_Block_brightness[s]; /* compare2 will store the result of Image_Block_brightness subtraction 
																					Symbol_Block_brightness */
				}
			
			if (Image_Block_brightness[b] < Symbol_Block_brightness[s]) /*if the Image_Block_brightness is smaller then the Symbol_Block_brightness*/
			{
				cmp2 = Symbol_Block_brightness[s] - Image_Block_brightness[b]; /* compare2 will store the result of Symbol_Block_brightness subtraction 
																					Image_Block_brightness */
																					
			}
			
			if (s == 0) /* if it is Symbol_Block_brightness [0] */ 
			{
				cmp1 = cmp2; /*compared variable one is equal to compare variable two */
				index = s; /* index equal to s, which is zero */
				} 
			if (cmp2 < cmp1) /* if compare two is smaller than the compare variable one */
			{
				cmp1 = cmp2; /*compare variable one equal to the compare variable two */
				index = s; /* index equal to s */
			}
			
			Replaced_symboluse[b] = symboluse[index]; /* an array of Replaced_symboluse will store all the replaced symbol */

			
		}


	}
		
}

void Mapping (char * Replaced_symboluse, SDL_Surface * image, TTF_Font * font, SDL_Surface * finaloutput )
{
		/* this function is use to create an new surface by create temporary surface into layer them and build them together */
		/* discussed and helped by Lucy Wright */
		
		SDL_Color color = { 255, 255, 255 }; /* set the colour to white */
		
		SDL_Surface* mapsurface = SDL_CreateRGBSurface(0, image->w, image->h, 32,0,0,0,255);//source from https://wiki.libsdl.org/SDL_CreateRGBSurface
		
																							//Function Parameters:
																							// 1. flags, be set to 0, 
																							// 2. the width of the surface
																							// 3. the height of the surface
																							// 4. the depth of the surface in bits
																							// 5. the red mask for the pixels
																							// 6. the green mask for the pixels
																							// 7. the blue mask for the pixels
																							// 8. the alpha mask for the pixels
		
		/*source from https://wiki.libsdl.org/SDL_Rect and https://wiki.libsdl.org/SDL_BlitSurface */
		
		SDL_Rect srcrect;
		srcrect.x = 0; /*the x location of the rectangle's upper left corner */
		srcrect.y = 0; /*the y location of the rectangle's upper left corner */
		srcrect.w = 6; /*the width of the rectangle */
		srcrect.h = 13; /* the height of the rectangle */
		
		SDL_Rect dstrect;
		dstrect.x = 0; /*the x location of the rectangle's upper left corner */
		dstrect.y = 0; /*the y location of the rectangle's upper left corner */
		dstrect.w = 6; /*the width of the rectangle */
		dstrect.h = 13; /* the height of the rectangle */
		
		
		int counter = 0; /* create a counter */
		
		
		
		for  (int y = 0; y <  image->h; y+=13)
			{
				for ( int x = 0; x < image->w; x+=6)
				{
					dstrect.x = x;
					dstrect.y = y;
					
					
					SDL_Surface * mapsurface = TTF_RenderText_Solid(font, &Replaced_symboluse[counter], color);
					/*create a temporary surface and name as "mapsurface"*/
																		   
				   /*Function Parameters: 
					1. Font to render the text with. A NULL pointer is not checked
					2. The LATIN1 null terminated string to render 
					3. The colour to render the text in. This becomes colourmap index 1 */
					
					SDL_BlitSurface(mapsurface, &srcrect, finaloutput, &dstrect); /*  perform a fast surface copy to a destination surface. */
																				
					/*Function Parameters: 
					1. src , the SDL_Surface structure to be copied from
					2. the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
					3. the SDL_Surface structure that is the blit target
					4. the SDL_Rect structure representing the rectangle that is copied into */
					
					counter ++; /* counter increase by 1 */
					
					
					
				}
			}
		
					SDL_FreeSurface(mapsurface);  /* free an RGB surface. */
}


/////////////////////////////////////////////enter main function///////////////////////////////////////////

	//source from http://www.programmersranch.com/2014/03/sdl2-displaying-text-with-sdlttf.html (import TTF library,
	// load font, Declare SDL_Surface,create a surface for ASCII characters, Declare SDL_ConvertSurfaceFormat,Close Window  )
	
	//source from http://www.programmersranch.com/2014/02/sdl2-converting-image-to-grayscale.html (import IMG and SDL library,
	//Create a Window, Declare SDL_Renderer, load image, Close Window)


int main(int argc, char ** argv)
{
    bool quit = false; 
    
////// Declare SDL_Event///////

    SDL_Event event; 
    
//////////////////////////////////////Import SDL, IMG, TTF Library//////////////////////////////////////////////////

    
    SDL_Init(SDL_INIT_VIDEO); //Import SDL library
    IMG_Init(IMG_INIT_PNG);	//Import SDL image library
    TTF_Init(); //Import SDL TTF library  
    
////////////////////////////// Ask for user to input the picture name in the file ////////////////////////////

    char picture [200];//the limited of the words that the user can type in terminal
    printf("input name of picture\n"); //ask user to input the picture name
    scanf("%s", picture); //read what the user have written
    fflush (stdin);//import the user input 
           
/////////////////////////////////////// Load image and font ///////////////////////////////////////////////////
    
    TTF_Font * font = TTF_OpenFont("LiberationMono-Regular.ttf", 10); //set font
    SDL_Color textcolor = { 255, 255, 255 };//set some ASCII characters that will be used later into white color
											//which mask value of Red , green and blue all to max value

/////////////////////////////////////////Declare SDL_Surface//////////////////////////////////////////////////
	
	SDL_Surface * image = IMG_Load(picture);//set image surface  as "image" regards to the image user imported
   
///////// Array of ASCII characters////////// 
        
    char symboluse[76] = {
						'A','B','C','D','E','F','G','H','I','J','K','L',
						'M','N','O','P','R','S','T','U','V','W','X','Y',
						'Z','a','b','c','d','e','f','g','h','i','j','k',
						'l','m','o','p','q','r','s','t','u','v','w','x',
						'y','z','!','#','$','%','&','(','*','+',',','-',
						'.',':','@','<',' ','1','2','3','4','5','6','7',
						'8','9','0','\0'
						
	};// name array of ASCII characters as symboluse 
	
///////////Create a pointer to point to the array of Symbol_Block_brightness////////////
	
	int *Symbol_Block_brightness = NULL; //set the pointer address to unused address
	Symbol_Block_brightness = (int*)malloc((image->w*image->h)*sizeof(int));// set the pointer to an array
	
////create a surface for ASCII characters//////
	
	//source from https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_43.html
	
    SDL_Surface * symbol = TTF_RenderText_Solid(font,symboluse,textcolor); //name the surface for ASCII characters as "symbol"
																		   
																		   //Function Parameters: 
																		   //1. Font to render the text with. A NULL pointer is not checked
																		   //2. The LATIN1 null terminated string to render 
																		   //3. The colour to render the text in. This becomes colormap index 1
																		   
	//source from https://wiki.libsdl.org/SDL_CreateRGBSurface
    
    SDL_Surface * finaloutput = SDL_CreateRGBSurface(0, image->w, image->h, 32,0,0,0,255);// create a new surface names as "finaloutput"
																						  
																						  //Function Parameters:
																						  // 1. flags, be set to 0, 
																						  // 2. the width of the surface
																						  // 3. the height of the surface
																						  // 4. the depth of the surface in bits
																						  // 5. the red mask for the pixels
																						  // 6. the green mask for the pixels
																						  // 7. the blue mask for the pixels
																						  // 8. the alpha mask for the pixels
																						  
///////////////////////////////////////Declare SDL_ConvertSurfaceFormat//////////////////////////////////////// 
   
	//source from https://wiki.libsdl.org/SDL_ConvertSurfaceFormat
	//to to access the pixel array of a loaded image we must convert it to a useful pixel format 
    
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0); //create "image" convert surface formate 
	symbol = SDL_ConvertSurfaceFormat(symbol, SDL_PIXELFORMAT_ARGB8888, 0); //create "symbol" convert surface formate 
	finaloutput = SDL_ConvertSurfaceFormat(finaloutput, SDL_PIXELFORMAT_ARGB8888, 0); //create "finaloutput" convert surface formate 

	//Function Parameters:
	// 1. the SDL_Surface structure representing the surface to convert
	// 2. pixel_format, which use DL_PIXELFORMAT_ARGB8888 for remarks
	// 3. the flags, and it should be set to 0


////////////////////////////////////////////Declare the SDL_PixelFormat/////////////////////////////////////////

	// source from https://wiki.libsdl.org/SDL_PixelFormat 
	//in here, we use Uint32 pixel formate
  
    Uint32 *symbolpixels = (Uint32*) symbol->pixels; //name pixels of symbols as "symbolpixels"
    Uint32 *pixels = (Uint32*) image->pixels;//name pixels of image as "pixels"
    
///////////////////////////////////////Create pointers to arrays///////////////////////////////////////////////    
     
    int Blocknum = (image->h*image->w)/(13*6); 

	//Malloc function ,source from http://www.tutorialspoint.com/c_standard_library/c_function_malloc.htm
      
    int * Image_Block_brightness = NULL;//set the pointer address to unused address
    Image_Block_brightness = (int*)malloc((image->w*image->h)*sizeof(int));// set the pointer to an array
        
    char * Replaced_symboluse = NULL;//set the pointer address to unused address
    Replaced_symboluse = (char*)malloc((image->w*image->h)*sizeof(char));// set the pointer to an array
    
////////////////////////////////////// Create a Window///////////////////////////////////////////////////////

	//source from https://wiki.libsdl.org/SDL_CreateWindow

    SDL_Window * window = SDL_CreateWindow("ASCII Art Image Conversion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

	//Function Parameters:
	//1. the title for the window 
	//2. the x position of the window 
	//3. the y position of the window  
	//4. the width of the window
	//5. the width of the window 
	//6. flags, set to 0
    
/////////////////////////////////////////Declare SDL_SetWindowSize////////////////////////////////////////////////	

	//source from https://wiki.libsdl.org/SDL_SetWindowSize

	SDL_SetWindowSize(window,image->w,image->h); // set window size to the size of image that user import  

	//Function Parameters:
	//1. the window i want to make change 
	//2. the width of the window in pixels, set it to the width of the image import 
	//3. the height of the window in pixels, set it to the height of the image import 
   
////////////////////////////////////// Declare SDL_Renderer///////////////////////////////////////////////////////

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);// source from https://wiki.libsdl.org/SDL_CreateRenderer
    
    // Function Parameters:
    
    //1. the window where rendering is displayed
    //2. -1 to initialize the first one supporting the requested flags
    //3. flags = 0
    
	SDL_Texture * finaltexture = SDL_CreateTextureFromSurface(renderer,finaloutput);//source from https://wiki.libsdl.org/SDL_CreateTextureFromSurface
	
	// Function Parameters:
	
	//1. the rendering context
	//2. the SDL_Surface structure containing pixel data used to fill the texture
    
    
///////////////////////////////// CALL THE FUNCTION AND PASS IN PARAMETERS //////////////////////////////////////
        	
    Symbol_analyse (font, textcolor, symbol, symbolpixels, Symbol_Block_brightness); 
    
    Symbol_bubblesort(Symbol_Block_brightness,symboluse );
    
    Symbol_Stretch (Symbol_Block_brightness);
    
    Image_analyse (image, pixels,Blocknum, Image_Block_brightness);
	 
    Replace (Symbol_Block_brightness, Image_Block_brightness, Blocknum, symboluse,Replaced_symboluse); 
        	   
	Mapping ( Replaced_symboluse, image, font, finaloutput );
	
	char ImageFileName [200];//the limited of the words that the user can type in terminal
    printf("input new name of picture\n"); //ask user to input the new picture name
    scanf("%s", ImageFileName); //read what the user have written
    fflush (stdin);
    
	IMG_SavePNG(finaloutput, ImageFileName); // image safe as PNG file
	

////////////////////////////////////////// CLOSE WINDOW ////////////////////////////////////////////////////////////
    
    while (!quit) // while the program not quit
    {
		//source from https://wiki.libsdl.org/SDL_UpdateTexture
		
		SDL_UpdateTexture(finaltexture, NULL, finaloutput->pixels, finaloutput->w * sizeof(Uint32));
		
		//Function Parameters:
		// 1. the texture to update
		// 2. an SDL_Rect structure representing the area to update, or NULL to update the entire texture
		// 3. the raw pixel data
		// 4. the number of bytes in a row of pixel data, including padding between lines
		
        
        
        SDL_WaitEvent(&event); //source from https://wiki.libsdl.org/SDL_WaitEvent

        switch (event.type) 
        {
        case SDL_QUIT: /* if press the X sign on the upper right hand side of the window */
            quit = true; /* the window will quit */
            break;
        }
		SDL_RenderClear(renderer); /*clear the current rendering target with the drawing colour */
		SDL_RenderCopy(renderer, finaltexture, NULL, NULL); /* copy a portion of the texture to the current rendering target */
		SDL_RenderPresent(renderer); /* update the screen with any rendering performed since the previous call */

    }

	SDL_DestroyTexture(finaltexture); /*destroy the specified texture */
    SDL_FreeSurface(image);/* free an RGB surface. */
	SDL_FreeSurface(symbol);/* free an RGB surface. */
	SDL_FreeSurface(finaloutput);/* free an RGB surface. */
    TTF_CloseFont(font);/* Free the memory used by font, and free font itself as well.*/
    SDL_DestroyRenderer(renderer); /* destroy the rendering context for a window and free associated textures. */
    SDL_DestroyWindow(window); /* destroy a window. */
    IMG_Quit(); /* quit SDL image library */
    TTF_Quit(); /* quit SDL TTF library */
    SDL_Quit(); /* quit SDL library */

    return 0; 
}

/////////////////////////////////////////////////////////////////END OF MAIN //////////////////////////////////////////////////////

