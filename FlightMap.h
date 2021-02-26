#pragma once
#include <string>

class FlightMap
{
	int cityNumber;
	int flightNumber;
	std::string citiesString;
	std::string flights;
	std::string* cityArr;
	std::string* flightNumbers;
	int** adjMatrix;
public:
	FlightMap(const std::string cityFile, const std::string flightFile);
	~FlightMap();
	void displayAllCities() const;
	void displayAdjacentCities(const std::string cityName) const;
	void displayFlightMap() const;
	void findFlights(const std::string deptCity, const std::string destCity) const;
	void findLeastCostFlight(const std::string deptCity, const std::string destCity) const;
	void runFlightRequests(const std::string requestFile) const;
	void displayMatrix() const;
	int findCity(const std::string cityName) const;
	void getAdjacentCities(const std::string& cityName, int& size, std::string*& resultArr) const;
	static bool isVisited(const std::string& cityName, std::string*& visitedCities, const int visitedSize);
	int findFlightNo(const std::string& dept, const std::string& dest) const;
};
