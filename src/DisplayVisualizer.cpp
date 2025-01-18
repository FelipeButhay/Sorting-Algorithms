#include "Display.h"

void Display::visualizerLoop() {
	if (IsMouseButtonPressed(0)) {
		// volver al menu
		if (mouseInRect(u, sy-2*u, MeasureText("Return", u), u)) {
			this->stage = 0;
			for (int i = 0; i<usingAlgorithmsN; i++) {
				this->cells[i].isSorting = false;
			}
			// limpia los threads
			for (int i = 0; i<usingAlgorithmsN; i++) {
				this->cells[i].thread.join();
				this->cells[i].array.clear();
				this->cells[i].alive = false;
			}
		}
	}
}

void Display::visualizerDraw() {
	ClearBackground(BACKGROUND);

	// dibuja todas las celulas
	for (int i = 0; i<usingAlgorithmsN; i++) {
		visualizeCell(&cells[i]);
	}

	DrawText("Return", u, sy-2*u, u, 
		mouseInRect(u, sy-2*u, MeasureText("Return", u), u) ? WHITE : GRAY);
}

void Display::visualizeCell(Cell* cell) {
	// crea un par de variable equivalentes a las que uso en la pantalla general
	int cu = cell->height*0.05f;
	Rectangle cb = { cell->x, cell->y, cell->width, cell->height };
	DrawRectangleRounded(cb, 0.05f, 0, LIGHTBLACK);
	 
	// escribe el nombre del algoritmo
	char* algorithmNameChar = strToChar(algorithms[cell->id]);

	// escribe la cantidad de elementos
	std::string sampleStr = (std::string)"n = " + intToStr(cell->array.size());
	char* sampleChar = strToChar(sampleStr);

	// si ya termino de ordenar usa el tiempo oficial, si no aproxima
	unsigned long long int msTime;
	if (cell->isSorting) {
		auto now = std::chrono::high_resolution_clock::now();
		msTime = (now - cell->executionTimeStart).count();
	} else {
		msTime = (cell->executionTime).count();
	}

	// calcula las distintas divisiones del fomato de tiempo por separado
	int minutes = (msTime/60000) % 1000;
	int seconds = (msTime/1000) % 60;
	int milliseconds = msTime % 1000;

	char timeChar[11] = { '0', '0', '0', ':', '0', '0', '.', '0', '0', '0', '\0' };

	timeChar[0] = 48 + (minutes/100)%10;
	timeChar[1] = 48 + (minutes/10)%10;
	timeChar[2] = 48 + (minutes/1)%10;

	timeChar[4] = 48 + (seconds/10)%10;
	timeChar[5] = 48 + (seconds/1)%10;

	timeChar[7] = 48 + (milliseconds/100)%10;
	timeChar[8] = 48 + (milliseconds/10)%10;
	timeChar[9] = 48 + (milliseconds/1)%10;

	if (cell->visual) {
		// dibujos si la celula es visual
		
		// textos
		DrawText(algorithmNameChar, cb.x + cu*4, cb.y + cu, cu*2, WHITE);
		DrawText(sampleChar, cb.x + cb.width - cu - MeasureText(sampleChar, cu*2), cb.y + cu, cu*2, WHITE);

		// simbolos de cargando / finalizado
		if (cell->isSorting) {
			Vector2 center = { cb.x + cu*2, cb.y + cu*2 };
			DrawRing(center, cu*0.75f, cu, 0, 360, 0, GRAY);
			DrawRing(center, cu*0.75f, cu, GetTime()*180 - (std::sin(GetTime()*PI) + 1.2f)*RAD2DEG*0.6f, GetTime()*180, 0, WHITE);

		} else {
			Vector2 p1 = { cb.x + cu      , cb.y + cu*1.66 };
			Vector2 p2 = { cb.x + cu*1.66f, cb.y + cu*2.33 };
			Vector2 p3 = { cb.x + cu*3    , cb.y + cu      };

			float thickness = cu*0.25f;
			DrawLineEx(p1, p2, thickness, GREEN);
			DrawLineEx(p2, p3, thickness, GREEN);
			DrawPoly(p2, 4, thickness*0.5f/std::cos(PI/4.0f), 0, GREEN);
		}

		// barras
		{
			std::lock_guard<std::mutex> lock(cell->mutex);

			int barW = (cb.width - 2*cu)/possibleSamples[sampleSize];
			int offset = (cb.width - barW*possibleSamples[sampleSize])/2;

			for (int i = 0; i < cell->array.size(); i++) {
				float barPerCent = cell->array[i]/(float)possibleSamples[sampleSize];
				int barH = cu*15.0f*barPerCent;

				DrawRectangle(cb.x + offset + barW*i, cb.y + cb.height - cu - barH, barW, barH, 
					(i == cell->iter1) || (i == cell->iter2) || (i == cell->iter3) || (i == cell->iter4) ? 
					WHITE : rainbowGen(360*barPerCent));
			}
		}
		 
	} else {
		// dibujos si la celula no es visual
		 
		// simbolos de cargando / finalizado
		if (cell->isSorting) {
			Vector2 center = { cb.x + cu*12, cb.y + cu*10 };
			DrawRing(center, cu*4.5f, cu*6.0f, 0, 360, 0, GRAY);
			DrawRing(center, cu*4.5f, cu*6.0f, GetTime()*180 - (std::sin(GetTime()*PI) + 1.2f)*RAD2DEG*0.6f, GetTime()*180, 0, WHITE);

		} else {
			Vector2 p1 = { cb.x + cu*6,  cb.y + cu*10 };
			Vector2 p2 = { cb.x + cu*10, cb.y + cu*14 };
			Vector2 p3 = { cb.x + cu*18, cb.y + cu*6  };

			float thickness = cu*1.5f;
			DrawLineEx(p1, p2, thickness, GREEN);
			DrawLineEx(p2, p3, thickness, GREEN);
			DrawPoly(p2, 4, thickness*0.5f/std::cos(PI/4.0f), 0, GREEN);
		}

		// textos
		DrawText(algorithmNameChar, cb.x + cu*24, cb.y + cu*6, cu*8, WHITE);
		DrawText(sampleChar, cb.x + cb.width - cu*12 - MeasureText("000:00.000", cu*8) - MeasureText(sampleChar, cu*8), 
			cb.y + cu*6, cu*8, WHITE);
		DrawText(timeChar, cb.x + cb.width - cu*6 - MeasureText(timeChar, cu*8), cb.y + cu*6, cu*8, WHITE);

	}

	delete[] algorithmNameChar;
	delete[] sampleChar;
}