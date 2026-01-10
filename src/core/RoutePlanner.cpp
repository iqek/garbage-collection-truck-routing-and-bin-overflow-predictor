/**
 * @file RoutePlanner.cpp
 * @brief Implementation of RoutePlanner class.
 * @author Miray Duygulu
 * @date 2025-12-28
 */
#include "core/RoutePlanner.h"

// Programda overflow kontrolü yapmak için başlangıç değerlerini çok küçük ya da çok büyük vermek için include ettik 
#include <climits> //INT_MAX


namespace project {

// Constructor
RoutePlanner::RoutePlanner(const Graph& graph)
    : graph(graph), predictor(2) {}

// Priority calculation
double RoutePlanner::calculatePriority(const Bin& bin, int distance) const {
    // Lower value = higher priority
    // Overflow risk dominates, distance is secondary
    double risk = predictor.getOverflowRisk(bin);  
    return risk * 1000 + distance; //we choose coefficent as 1000 due to 1000 > max_distance
}

// Dijkstra shortest path
int RoutePlanner::computeDistance(int from, int to) const {
    
    int nodeCount = graph.getNodeCount();   //Graph içindeki toplam node sayısını alır

    int* distance = new int[nodeCount];
    bool* visited = new bool[nodeCount];

    for (int i = 0; i < nodeCount; i++) {
        distance[i] = INT_MAX;  // tüm node'ların mesafesini sonsuz kabul ettik (INT_MAX)
        visited[i] = false;     // Hiçbir node ziyaret edilmemiş kabul edilir
    }

    PriorityQueue<int> shortestDistanceQueue;
    distance[from] = 0;
    shortestDistanceQueue.push(from, 0);

    while (!shortestDistanceQueue.isEmpty()) {
        int current = shortestDistanceQueue.top(); // en yakın node'u bulur
        shortestDistanceQueue.pop();

        if (visited[current]) continue;     //visited ise geç
        visited[current] = true;            //visited olarak işaretle

        if (current == to) break;           // break if the target node is reached 

        const LinkedList<Edge>& edges = graph.getAdjList(current);  //current node’unun tüm komşu kenarları
        for (auto it = edges.begin(); it != edges.end(); ++it) {        //current node’dan çıkan her kenar tek tek incelenir
            Edge e = *it;
            int next = e.toNode;    //gidilecek komşu node
            int weight = e.weight;  //next ile arasındaki mesafe

            if (!visited[next] && distance[current] + weight < distance[next]) {    //Daha kısa yol var mı kontrol eder
                distance[next] = distance[current] + weight;    //Yeni ve daha kısa mesafe kaydedilir
                shortestDistanceQueue.push(next, distance[next]);
            }
        }
    }

    int result = distance[to];  //Hedef node’a olan en kısa mesafe
    
    //Memory cleanup
    delete[] distance;
    delete[] visited;

    return result;
}

// Find nearest disposal facility
int RoutePlanner::findNearestDisposal(int currentNode,
                                      const Facilities& facilities) const {
    int count = 0;
    int* disposalNodes = facilities.getDisposalNodes(count);    //facilities içinden disposal node ID’lerini alır

    if (count == 0) return -1;  // Disposal tesisi yoksa -1 ile fonksiyondan çıkılır

    int minDistance = INT_MAX; // we assume infinity at first
    int nearest = -1;

    for (int i = 0; i < count; i++) {   // Her disposal node kontrol edilir
        int dijkstra = computeDistance(currentNode, disposalNodes[i]);  // en kısa mesafeyi hesaplar
        if (dijkstra < minDistance) {   
            minDistance = dijkstra;     
            nearest = disposalNodes[i]; //  disposal node ID’si saklanır
        }
    }

    delete[] disposalNodes; //Memory cleanup
    return nearest;
}

// Check for critical bins
bool RoutePlanner::hasCriticalBins(const Facilities& facilities) const {
    for (int i = 0; i < facilities.getBinCount(); i++) {    
        if (predictor.isCritical(facilities.getBin(i))) {
            return true;
        }
    }
    return false;
}

// Select next bin (greedy)
int RoutePlanner::selectNextBin(Facilities& facilities) {
    Truck& truck = facilities.getTruck();   //Facilities içindeki truck’a referans alır (no copy)
    int currentNode = truck.getCurrentNode();   //Truck’ın graph üzerindeki mevcut konumu

    double bestScore = INT_MAX;
    int bestIndex = -1;

    for (int i = 0; i < facilities.getBinCount(); i++) {
        Bin& bin = facilities.getBin(i);

        if (bin.getCurrentFill() == 0) continue;    //eğer bin boşsa atla

        int distance = computeDistance(currentNode, bin.getNodeId());
        double score = calculatePriority(bin, distance);    //bin'in öncelik skoru hesaplanır

        if (score < bestScore) {    //daha öncelikli var mı kontrol edilir
            bestScore = score;
            bestIndex = i;
        }
    }

    return bestIndex;
}

// Plan full route
Route RoutePlanner::planRoute(Facilities& facilities) {
    Route route;    // Initialize an empty route
    Truck& truck = facilities.getTruck();

    int currentNode = facilities.getDepotNode();    //Truck’ın başlangıç noktası olan depot node’u alınır
    truck.moveTo(currentNode);      //Truck’ın konumu depot olarak ayarlanır

    while (true) {
        int nextBinIndex = selectNextBin(facilities);
        if (nextBinIndex == -1) break;

        Bin& bin = facilities.getBin(nextBinIndex);

        // Check capacity
        if (bin.getCurrentFill() > truck.getRemainingCapacity()) {
            int disposalNode =      // Mevcut konuma göre en yakın boşaltma tesisi node’u
                findNearestDisposal(truck.getCurrentNode(), facilities);
            if (disposalNode != -1) {   //valid dispol bulunduysa
                route.setNeedsDisposal(true);   //Bu rotada boşaltma yapılacağını belirtir
                currentNode = disposalNode;
                truck.moveTo(currentNode);
                truck.unload();
                
            }
            continue;
        }

        // Visit bin
        route.addBin(nextBinIndex);             //Bin rota listesine eklenir
        truck.collect(bin.getCurrentFill());    //Bin’deki mevcut atık truck’a yüklenir
        currentNode = bin.getNodeId();          
        truck.moveTo(currentNode);      //Truck bin’in bulunduğu node’a gider
        bin.collect(bin.getCurrentFill());      //Bin’in içi boşaltılır (fill = 0)
    }

    return route;
}

} // namespace project
