#include "FlightMap.h"
#include "FlightStack.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;
/*TODO:
 * memory leak at findflights
 * complete findflight to delete the correct way (not the first flight before result)
 */
FlightMap::FlightMap(const string cityFile, const string flightFile)
{
	citiesString = "";
	string city;
	fstream cityRead(cityFile.c_str());
	cityNumber = 0;
	
	while (getline(cityRead, city, '\r'))
	{
		citiesString += city + "\n";
		cityNumber++;
		getline(cityRead, city);
	}/*
	while (getline(cityRead, city))
	{
		citiesString += city + "\n";
		cityNumber++;
	}*/
	cityRead.close();
	
	cityArr = new string[cityNumber];
	stringstream lineSeperator(citiesString);
	int index = 0;
	while (getline(lineSeperator, city))
	{
		cityArr[index] = city;
		index++;
	}
	
	fstream flightRead(flightFile.c_str());
	flights = "";
	string flight;
	flightNumber = 0;
	
	while (getline(flightRead, flight, '\r'))
	{
		flights += flight + "\n";
		flightNumber++;
		getline(flightRead, flight);
	}
	/*while (getline(flightRead, flight))
	{
		flights += flight + "\n";
		flightNumber++;
	}*/
	adjMatrix = new int*[cityNumber];
	for (int i = 0; i < cityNumber; i++)
	{
		adjMatrix[i] = new int[cityNumber];
		for (int j = 0; j < cityNumber; j++)
		{
			adjMatrix[i][j] = 0;
		}
	}
	flightRead.close();
	
	stringstream lineSeperator2(flights);
	string flightLine;
	int flightCount = 0;
	while (getline(lineSeperator2, flightLine))
	{
		stringstream line(flightLine);
		string character;
		string lineArr[4];
		int lineIndex = 0;
		while (getline(line, character, ','))
		{
			lineArr[lineIndex] = character;
			lineIndex++;
		}
		string firstCity = lineArr[0];
		string secondCity = lineArr[1];
		stringstream ss;
		int price;
		ss << lineArr[3];
		ss >> price;
		int firstCityIndex = findCity(firstCity);
		int secondCityIndex = findCity(secondCity);
		adjMatrix[firstCityIndex][secondCityIndex] = price;
		flightCount++;
	}
	
	flightNumbers = new string[flightCount * 3];
	int flightIndex = 0;
	stringstream flightSeperator(flights);
	while (getline(flightSeperator, flightLine))
	{
		stringstream line(flightLine);
		string character;
		string lineArr[4];
		int lineIndex = 0;
		while (getline(line, character, ','))
		{
			lineArr[lineIndex] = character;
			lineIndex++;
		}
		flightNumbers[flightIndex * 3] = lineArr[0];
		flightNumbers[flightIndex * 3 + 1] = lineArr[1];
		flightNumbers[flightIndex * 3 + 2] = lineArr[2];
		flightIndex++;
	}
}

FlightMap::~FlightMap()
{
	for (int i = 0; i < cityNumber; i++)
	{
		delete[] adjMatrix[i];
	}
	delete[] adjMatrix;
	delete[] cityArr;
	delete[] flightNumbers;
}

void FlightMap::displayAllCities() const
{
	for (int i = 0; i < cityNumber; i++)
	{
		cout << cityArr[i] << ", ";
	}
	cout << endl;
}

void FlightMap::displayAdjacentCities(const string cityName) const
{
	cout << cityName << " -> ";
	int indexOfCity = findCity(cityName);
	for (int i = 0; i < cityNumber; i++)
	{
		if (adjMatrix[indexOfCity][i] != 0)
		{
			cout << cityArr[i] << ", ";
		}
	}
	cout << endl;
}

void FlightMap::displayFlightMap() const
{
	for (int i = 0; i < cityNumber; i++)
	{
		displayAdjacentCities(cityArr[i]);
	}
}

void FlightMap::findFlights(const string deptCity, const string destCity) const
{
	cout << "Request is to fly from " << deptCity << " to " << destCity << ":" << endl;
	FlightStack st;
	st.push(deptCity);
	string* visited = new string[cityNumber];
	visited[0] = deptCity;
	int visitedIndex = 1;
	string topCity;
	int adjacentIndex = 0;
	bool isFinished = false;
	bool isFound = false;
	string* adjacents;
	while (!st.isEmpty() && st.getTop(topCity))
	{
		if (topCity != destCity)
		{
			int adjacentNumber;
			getAdjacentCities(topCity, adjacentNumber, adjacents);
			if (adjacentNumber == 0 || adjacentIndex == adjacentNumber || isVisited(adjacents[adjacentIndex], visited, visitedIndex) || isFound)
			{
				adjacentIndex++;
				if (adjacentNumber == 0 || adjacentIndex == adjacentNumber || isFound)
				{
					isFound = false;
					st.pop();
					adjacentIndex = 0;
				}
			}
			else
			{
				st.push(adjacents[adjacentIndex]);
				visited[visitedIndex] = adjacents[adjacentIndex];
				visitedIndex++;
				adjacentIndex = 0;
			}
			delete[] adjacents;
		}
		else
		{
			int total = 0;
			FlightStack temp = st;
			do
			{
				string top;
				string underTop;
				temp.pop(top);
				temp.getTop(underTop);
				int depIndex = findCity(underTop);
				int desIndex = findCity(top);
				if (depIndex >= 0 && desIndex >= 0)
				{
					cout << "Flight #" << findFlightNo(underTop, top) << " from " << underTop << " to " << top << "\t" << "Cost: " << adjMatrix[depIndex][desIndex] << endl;
					total += adjMatrix[depIndex][desIndex];
				}
			}
			while (!temp.isEmpty());
			cout << "Total Cost .......... " << total << endl << endl;
			st.pop();
			isFound = true;
			isFinished = true;
			for (int i = 0; i < cityNumber; i++)
			{
				if (visited[i] == destCity)
				{
					visited[i] = "";
					visitedIndex--;
				}
			}
		}
	}
	if (st.isEmpty() && !isFinished)
	{
		cout << "Sorry. HPAir does not fly from " << deptCity << " to " << destCity << endl;
	}
	delete[] visited;
}

void FlightMap::findLeastCostFlight(const string deptCity, const string destCity) const
{
	int minPrice = 0;
	FlightStack roadStack;
	cout << "Request is to fly from " << deptCity << " to " << destCity << ":" << endl;
	FlightStack st;
	st.push(deptCity);
	string* visited = new string[cityNumber];
	int visitedIndex = 0;
	visited[visitedIndex] = deptCity;
	visitedIndex++;
	string topCity;
	int adjacentIndex = 0;
	bool isFinished = false;
	bool isFound = false;
	string* adjacents;
	int solutionNumber = 0;
	while (!st.isEmpty() && st.getTop(topCity))
	{
		if (topCity != destCity)
		{
			int adjacentNumber;
			getAdjacentCities(topCity, adjacentNumber, adjacents);
			if (adjacentNumber == 0 || adjacentIndex == adjacentNumber || isVisited(adjacents[adjacentIndex], visited, visitedIndex) || isFound)
			{
				adjacentIndex++;
				if (adjacentNumber == 0 || adjacentIndex == adjacentNumber || isFound)
				{
					isFound = false;
					st.pop();
					adjacentIndex = 0;
				}
			}
			else
			{
				st.push(adjacents[adjacentIndex]);
				visited[visitedIndex] = adjacents[adjacentIndex];
				visitedIndex++;
				adjacentIndex = 0;
			}
			delete[] adjacents;
		}
		else
		{
			int total = 0;
			FlightStack temp = st;
			string road;
			do
			{
				string top;
				string underTop;
				temp.pop(top);
				temp.getTop(underTop);
				int depIndex = findCity(underTop);
				int desIndex = findCity(top);
				if (depIndex >= 0 && desIndex >= 0)
				{
					cout << "Flight #" << findFlightNo(underTop, top) << " from " << underTop << " to " << top << "\t" << "Cost: " << adjMatrix[depIndex][desIndex] << endl;
					road += top + ",";
					total += adjMatrix[depIndex][desIndex];
				}
			}
			while (!temp.isEmpty());
			cout << "Total Cost .......... " << total << endl << endl;
			st.pop();
			isFound = true;
			isFinished = true;
			stringstream ss;
			ss << total;
			string totalPrice;
			ss >> totalPrice;
			road += deptCity + "!";
			road += totalPrice;
			roadStack.push(road);
			if (solutionNumber == 0)
			{
				minPrice = total;
			}
			else
			{
				if (total < minPrice)
				{
					minPrice = total;
				}
			}
			solutionNumber++;
			for (int i = 0; i < cityNumber; i++)
			{
				if (visited[i] == destCity)
				{
					visited[i] = "";
					visitedIndex--;
				}
			}
		}
	}
	if (st.isEmpty() && !isFinished)
	{
		cout << "Sorry. HPAir does not fly from " << destCity << " to " << deptCity << endl;
		return;
	}
	delete[] visited;
	string road;
	bool isMinimum = false;
	while (!isMinimum)
	{
		roadStack.getTop(road);
		stringstream ss;
		int roadPrice;
		int indexOfPrice = road.find_first_of("!");
		ss << road.substr(indexOfPrice + 1);
		ss >> roadPrice;
		if (roadPrice == minPrice)
		{
			cout << "A least cost path from " << deptCity << " to " << destCity << " is ";
			isMinimum = true;
			string cities = road.substr(0, indexOfPrice);
			cities = string(cities.rbegin(), cities.rend());
			size_t found = cities.find_first_of(",");
			while (found != string::npos)
			{
				cout << cities.substr(0, found) << " -> ";
				cities = cities.substr(found + 1);
				found = cities.find_first_of(",");
			}
			cout << destCity << " and its cost is " << roadPrice << " TL" << endl;
			return;
		}
		roadStack.pop();
	}
}

void FlightMap::runFlightRequests(const string requestFile) const
{
	string requestString = "";
	string request;
	fstream reqRead(requestFile.c_str());
	
	while (getline(reqRead, request, '\r'))
	{
		size_t found = request.find_first_of(",");
		string deptCity = request.substr(0, found);
		string destCity = request.substr(found + 1);
		findLeastCostFlight(deptCity, destCity);
		cout << endl;
		getline(reqRead, request); 
	}
	/*while (getline(reqRead, request))
	{
		size_t found = request.find_first_of(",");
		string deptCity = request.substr(0, found);
		string destCity = request.substr(found + 1);
		findLeastCostFlight(deptCity, destCity);
		cout << endl;
	}*/
	reqRead.close();
}

void FlightMap::displayMatrix() const
{
	stringstream lines(citiesString);
	stringstream lines2(citiesString);
	string city;
	cout << setw(12) << "";
	while (getline(lines, city))
	{
		cout << left << setw(11) << city << " ";
	}
	cout << endl;
	int i = 0, j;
	while (getline(lines2, city))
	{
		cout << setw(11) << city << " ";
		for (j = 0; j < cityNumber; j++)
		{
			cout << left << setw(11) << adjMatrix[i][j] << " ";
		}
		i++;
		cout << endl;
	}
}

int FlightMap::findCity(const string cityName) const
{
	for (int i = 0; i < cityNumber; i++)
	{
		if (cityName == cityArr[i])
		{
			return i;
		}
	}
	return -1;
}

void FlightMap::getAdjacentCities(const string& cityName, int& size, string*& resultArr) const
{
	resultArr = new string[cityNumber];
	size = 0;
	const int indexOfCity = findCity(cityName);
	if (indexOfCity >= 0)
	{
		for (int i = 0; i < cityNumber; i++)
		{
			if (adjMatrix[indexOfCity][i] != 0)
			{
				resultArr[size] = cityArr[i];
				size++;
			}
		}
	}
}

bool FlightMap::isVisited(const string& cityName, string*& visitedCities, const int visitedSize)
{
	for (int i = 0; i < visitedSize; i++)
	{
		if (cityName == visitedCities[i])
		{
			return true;
		}
	}
	return false;
}

int FlightMap::findFlightNo(const string& dept, const string& dest) const
{
	for (int i = 0; i < flightNumber; i++)
	{
		string deptCity = flightNumbers[3 * i];
		string destCity = flightNumbers[3 * i + 1];
		if (deptCity == dept && destCity == dest)
		{
			stringstream ss;
			int flightNo;
			ss << flightNumbers[3 * i + 2];
			ss >> flightNo;
			return flightNo;
		}
	}
	return -1;
}
