#include "Display.h"

void Display::menuLoop() {
	if (IsKeyPressed(KEY_SPACE)) {
		if (GetWindowPosition().x == 0 && GetWindowPosition().y == 0) {
			SetWindowPosition(100, 100);
		} else {
			SetWindowPosition(0, 0);
		}
	}

	if (mouseInRect(u, 5*u, sx*0.5f - u*1.5f, algorithmsInScreenMD*u)) {
		// movimiento de la rueda
		float wheelMove = GetMouseWheelMove();
		if (wheelMove != 0) {
			this->topAlgorithmMD -= wheelMove;

			// limites de la rueda
			if (topAlgorithmMD < topAlgorithmBoundsMD[0]) {
				this->topAlgorithmMD = topAlgorithmBoundsMD[0];
			}
			if (topAlgorithmMD > topAlgorithmBoundsMD[1]) {
				this->topAlgorithmMD = topAlgorithmBoundsMD[1];
			}

			// actualizacion de la posicion de la barra de scroll
			this->barPercentagePos = topAlgorithmMD/((float)algorithmsN);
		}
	}

	if (IsMouseButtonPressed(0)) {
		if (mouseInCirc(sx - u*2.0f, u*1.95f, u)) {
			this->stage = 1;
		}

		if (mouseInRect(sx*0.5f - u*1.0f, 5*u + barSize*barPercentagePos, u*0.5f, barSize*barPercentageSize)) {
			// coordenada inicial y top algorithm al clickear en la barra
			this->clickBarY = GetMouseY();
			this->topAlgorithmMDSave = topAlgorithmMD;
			this->barMoving = true;
		} else {
			this->barMoving = false;
		}

		// agrega los algoritmos a la lista de seleccionados
		for (int i = 0; i<algorithmsInScreenMD; i++) {
			if (mouseInRect(sx*0.5f - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f) && usingAlgorithmsN < 4) {
				this->usingAlgorithmsID[usingAlgorithmsN] = i + topAlgorithmMD;
				this->usingAlgorithmsN++;
			}
		}

		for (int i = 0; i<4; i++) {
			// elimina los algoritmos
			if (mouseInRect(sx - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f)) {
				this->usingAlgorithmsID[i] = -1;
				this->usingAlgorithmsN--;
			}
			 
			// corre para atraz si hay un espacio vacio
			if (usingAlgorithmsID[i] == -1) {
				this->usingAlgorithmsID[i] = usingAlgorithmsID[i+1];
				this->usingAlgorithmsID[i+1] = -1;
			}
		}

		if (mouseInRect(sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + 3*u, sx*0.5f - u*1.5f, 2*u) && usingAlgorithmsN != 0) {
			this->stage = 2;

			for (int i = 0; i<4; i++) {
				this->cells[i].alive = false;
			}

			// crea distintos tipos de celula dependiendo de si la tiene que graficar o no
			if (visualizerTime == 0) {
				// celdas cuando no es visual
				for (int i = 0; i<usingAlgorithmsN; i++) {
					this->cells[i].x      = u;
					this->cells[i].y      = (sy - u*7)*0.25f*i + u*(i+1);
					this->cells[i].width  =  sx - u*2;
					this->cells[i].height = (sy - u*7)*0.25f;

					this->cells[i].id = usingAlgorithmsID[i];
								
					this->cells[i].visual = false;
					this->cells[i].alive  = true;
				}
			} else {
				// celdas cuando es visual
				for (int i = 0; i<usingAlgorithmsN; i++) {
					switch (usingAlgorithmsN){
						case 1:
							this->cells[i].x = u;
							this->cells[i].y = u;
							this->cells[i].width = sx - u*2;
							this->cells[i].height = sy - u*4;
							break;
						case 2:
							this->cells[i].x = u;
							this->cells[i].y = (sy - u*5)*0.5f*i + u*i + u;
							this->cells[i].width = sx - u*2;
							this->cells[i].height = (sy - u*5)*0.5f;
							break;
						case 3:
						case 4:
							this->cells[i].x = (sx - u*3)*0.5f*(i%2) + u*(i%2) + u;
							this->cells[i].y = (sy - u*5)*0.5f*(i/2) + u*(i/2) + u;
							this->cells[i].width = (sx - u*3)*0.5f;
							this->cells[i].height = (sy - u*5)*0.5f;
							break;
					}

					this->cells[i].interval = possibleTimes[visualizerTime];
				   
					this->cells[i].id = usingAlgorithmsID[i];
				   
					this->cells[i].visual = true;
					this->cells[i].alive  = true;
				}
			}

			// crea un array ordenado del 1 al n;
			std::vector<int> initialVector(possibleSamples[sampleSize]);
			std::vector<int> rNumberSequence(possibleSamples[sampleSize]);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, possibleSamples[sampleSize] - 1);

			for (int i = 0; i<possibleSamples[sampleSize]; i++) {
				initialVector[i] = i+1;
				rNumberSequence[i] = dis(gen);
			}
			
			// se lo asigna a cada celula y pone a andar cada hilo
			for (int i = 0; i<usingAlgorithmsN; i++) {
				this->cells[i].iter1 = -1;
				this->cells[i].iter2 = -1;
				this->cells[i].iter3 = -1;
				this->cells[i].iter4 = -1;
				this->cells[i].iter5 = -1;

				this->cells[i].array = initialVector;
				this->cells[i].rNumbers = rNumberSequence;

				this->cells[i].algorithmFunc = algorithmFuncPtr[cells[i].id];
				this->cells[i].algorithmFuncVisual = algorithmFuncVisualPtr[cells[i].id];
				this->cells[i].thread = std::thread(&Display::measureAlgorithmTime, this, &cells[i]);
			}
		}
	}

	if (IsMouseButtonDown(0)) {
		if (mouseInRect(sx*0.5f - u*1.0f, 5*u, u*0.5f, algorithmsInScreenMD*lineSizeU*u) && barMoving) {

			// sacar cuantas lineas se movio
			int deltaBarLines = (GetMouseY() - clickBarY)/(u*1.4f*barPercentageSize);
			this->topAlgorithmMD = topAlgorithmMDSave + deltaBarLines;

			// limites de la rueda
			if (topAlgorithmMD < topAlgorithmBoundsMD[0]) {
				this->topAlgorithmMD = topAlgorithmBoundsMD[0];
			}
			if (topAlgorithmMD > topAlgorithmBoundsMD[1]) {
				this->topAlgorithmMD = topAlgorithmBoundsMD[1];
			}

			// actualizacion de la posicion de la barra de scroll
			this->barPercentagePos = topAlgorithmMD/((float)algorithmsN);
		} else {
			this->barMoving = false;
		}

		// sample slider
		if (mouseInRect(sx*0.5f + u*0.2f, algorithmsInScreenMD*lineSizeU*u - u*1.7f, sx*0.5f - u*0.9f, u*0.6f)) {
			float sampleSliderPercentage = (GetMouseX() - sx*0.5f - u*0.2f) / (sx*0.5f - u*0.9f);
			this->sampleSize = sampleSliderPercentage * samplesN;
		}

		// time slider
		if (mouseInRect(sx*0.5f + u*0.2f, algorithmsInScreenMD*lineSizeU*u + u*1.3f, sx*0.5f - u*0.9f, u*0.6f)) {
			float timeSliderPercentage = (GetMouseX() - sx*0.5f - u*0.2f) / (sx*0.5f - u*0.9f);
			this->visualizerTime = timeSliderPercentage * timesN;
		}
	}
}

void Display::menuDraw() {
	ClearBackground(BACKGROUND);

	// Titles
	DrawText("Sorting Algorithms Visualizer", u*1.2f, u*1.2f, u*1.5f, BLACK);
	DrawText("Sorting Algorithms Visualizer",      u,      u, u*1.5f, WHITE);

	DrawText("Available Algorithms", u               , 3.5f*u, u, WHITE);
	DrawText("Selected Algorithms" , sx*0.5f + u*0.5f, 3.5f*u, u, WHITE);

	Vector2 SettingsPos = { sx - u*2.0f, u*1.95f };
	DrawPoly(SettingsPos, 6, u, 0, 
		mouseInCirc(sx - u*2.0f, u*1.95f, u) ? DARKGRAY : GRAY);
	DrawCircleV(SettingsPos, u*0.4f, BACKGROUND);

	// >> AVAILABLE ALGORITHMS <<
	DrawRectangle(u, 5*u, sx*0.5f - u*1.5f, algorithmsInScreenMD*lineSizeU*u, LIGHTBLACK);
	DrawRectangle(sx*0.5f - u*1.0f, 5*u, u*0.5f, algorithmsInScreenMD*lineSizeU*u, GRAY);

	// dibuja la barra del scroll moviendose o quieta si no hay suficientes algoritmos
	DrawRectangle(sx*0.5f - u*1.0f, 5*u + barSize*barPercentagePos, u*0.5f, barSize*barPercentageSize, WHITE);

	// dibuja los nombres de los algoritmos
	for (int i = 0; i<algorithmsInScreenMD; i++) {
		int indx = i + topAlgorithmMD;

		//rompe el loop si se pasa (cuando no hay suficientes algoritmos)
		if (indx >= algorithmsN) {
			break;
		}

		// el nombre
		char* algorithmName = strToChar(algorithms[indx]);
		DrawText(algorithmName, u*1.4f, u*5.3f + i*u*lineSizeU, u*0.8, WHITE);
		delete[] algorithmName;

		// tapa un poco el nombre si es muy largo
		DrawRectangle(sx*0.5f - u*3.0f, u*5.0f + i*u*lineSizeU, u*2, u*lineSizeU, LIGHTBLACK);

		// boton de + si queda espacio en la lista de seleccionados
		if (usingAlgorithmsN < 4) {
			DrawRectangle(sx*0.5f - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f, 
				mouseInRect(sx*0.5f - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f) ? DARKGREEN : GREEN);

			DrawText("+", sx*0.5f - u*2.0f - MeasureText("+", u*0.8f)/2, u*5.3f + i*u*lineSizeU, u*0.8f, 
				mouseInRect(sx*0.5f - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f) ? BLACK : LIGHTBLACK);
		}
	}

	// >> SELECTED ALGORITHMS <<
	DrawRectangle(sx*0.5f + u*0.5f, 5*u, sx*0.5f - u*1.5f, 4*lineSizeU*u, LIGHTBLACK);

	// nombres seleccionados
	for (int i = 0; i<usingAlgorithmsN; i++) {
		// el nombre
		char* algorithmName = strToChar(algorithms[usingAlgorithmsID[i]]);
		DrawText(algorithmName, sx*0.5f + u*0.9f, u*5.3f + i*u*lineSizeU, u*0.8, WHITE);
		delete[] algorithmName;

		// tapa un poco el nombre si es muy largo
		DrawRectangle(sx - u*3.0f, u*5.0f + i*u*lineSizeU, u*2, u*lineSizeU, LIGHTBLACK);

		// boton de -
		DrawRectangle(sx - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f, 
			mouseInRect(sx - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f) ? DARKRED : RED);

		DrawText("-", sx - u*2.0f - MeasureText("-", u*0.8f)/2, u*5.3f + i*u*lineSizeU, u*0.8f, 
			mouseInRect(sx - u*2.5f, u*5.2f + i*u*lineSizeU, u*1, u*1.0f) ? BLACK : LIGHTBLACK);
	}

	// >> SLIDERS <<
	// slider de la cantidad de numeros
	// titulo
	std::string sampleSliderEndStr = " numbers to order.";
	std::string sampleSliderFullStr = possibleSamplesStr[sampleSize] + sampleSliderEndStr;
	char* sampleSliderFullChar = strToChar(sampleSliderFullStr);
	DrawText(sampleSliderFullChar, sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u - u*3, u*0.8f, WHITE);
	delete[] sampleSliderFullChar;

	//slider
	int sampleSliderPosX = (sx*0.5f - u*1.5f) * sampleSize / (float)(samplesN - 1);
	Rectangle sampleSliderRoundRec = { sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u - u*1.5f, sx*0.5f - u*1.5f, u*0.2f};
	Rectangle sampleSliderRoundRecPartial = { sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u - u*1.5f, sampleSliderPosX, u*0.2f };
	DrawRectangleRounded(sampleSliderRoundRec, 1, 0, GRAY);
	DrawRectangleRounded(sampleSliderRoundRecPartial, 1, 0, WHITE);

	DrawCircle(sampleSliderRoundRec.x + sampleSliderPosX, sampleSliderRoundRec.y + u*0.1f, u*0.3f, WHITE);

	// slider del tiempo
	// titulo
	std::string timeSliderEndStr = visualizerTime > 9 ? " second/s interval." : " millisecond/s interval.";
	std::string timeSliderFullStr = possibleTimesStr[visualizerTime] + timeSliderEndStr;
	char* timeSliderFullChar = strToChar(timeSliderFullStr);
	DrawText(timeSliderFullChar, sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u, u*0.8f, WHITE);
	delete[] timeSliderFullChar;

	//slider
	int timeSliderPosX = (sx*0.5f - u*1.5f) * visualizerTime / (float)(timesN - 1);
	Rectangle timeSliderRoundRec = { sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + u*1.5f, sx*0.5f - u*1.5f, u*0.2f };
	Rectangle timeSliderRoundRecPartial = { sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + u*1.5f, timeSliderPosX, u*0.2f };

	DrawRectangleRounded(timeSliderRoundRec, 1, 0, GRAY);
	DrawRectangleRounded(timeSliderRoundRecPartial, 1, 0, WHITE);

	DrawCircle(timeSliderRoundRec.x + timeSliderPosX, timeSliderRoundRec.y + u*0.1f, u*0.3f, WHITE);

	// >> RUN BUTTON <<
	DrawRectangle(sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + 3*u, sx*0.5f - u*1.5f, 2*u,
		mouseInRect(sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + 3*u, sx*0.5f - u*1.5f, 2*u) ? DARKGREEN : GREEN);

	DrawText("RUN", sx*0.75f - u*0.25f - MeasureText("RUN", u*1.4f)*0.5f, algorithmsInScreenMD*lineSizeU*u + u*3.3f, u*1.4f,
		mouseInRect(sx*0.5f + u*0.5f, algorithmsInScreenMD*lineSizeU*u + 3*u, sx*0.5f - u*1.5f, 2*u) ? BLACK : LIGHTBLACK);
}