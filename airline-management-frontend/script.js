document.addEventListener("DOMContentLoaded", () => {
  const homeSection = document.getElementById("home");
  const adminDashboard = document.getElementById("adminDashboard");
  const passengerDashboard = document.getElementById("passengerDashboard");
  const adminContent = document.getElementById("adminContent");
  const passengerContent = document.getElementById("passengerContent");

  // Navigation
  document.getElementById("adminBtn").addEventListener("click", () => {
    homeSection.classList.add("hidden");
    adminDashboard.classList.remove("hidden");
  });

  document.getElementById("passengerBtn").addEventListener("click", () => {
    homeSection.classList.add("hidden");
    passengerDashboard.classList.remove("hidden");
  });

  document.querySelectorAll(".backBtn").forEach((btn) => {
    btn.addEventListener("click", () => {
      adminDashboard.classList.add("hidden");
      passengerDashboard.classList.add("hidden");
      homeSection.classList.remove("hidden");
    });
  });

  // Admin Dashboard
  document.getElementById("addFlightBtn").addEventListener("click", () => {
    adminContent.innerHTML = `
      <h3>Add Flight</h3>
      <form id="addFlightForm">
        <label>Company: <input type="text" id="company"></label><br>
        <label>Flight Number: <input type="text" id="flightNumber"></label><br>
        <label>Source: <input type="text" id="source"></label><br>
        <label>Destination: <input type="text" id="destination"></label><br>
        <label>Total Seats: <input type="number" id="totalSeats"></label><br>
        <button type="submit">Add Flight</button>
      </form>
    `;
    document.getElementById("addFlightForm").addEventListener("submit", (e) => {
      e.preventDefault();
      const flight = {
        company: document.getElementById("company").value,
        flightNumber: document.getElementById("flightNumber").value,
        source: document.getElementById("source").value,
        destination: document.getElementById("destination").value,
        totalSeats: document.getElementById("totalSeats").value,
      };
      // Send flight data to the backend (API call)
      fetch('http://localhost:5001/api/flights/add', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(flight)
      })
        .then(response => response.json())
        .then(data => {
          console.log("Flight added:", data);
          alert("Flight added successfully!");
        })
        .catch(error => {
          console.error("Error adding flight:", error);
          alert("Error adding flight.");
        });
    });
  });

  document.getElementById("viewFlightsBtn").addEventListener("click", () => {
    adminContent.innerHTML = "<h3>All Flights</h3>";

    fetch('http://localhost:5001/api/flights')
      .then(response => response.json())
      .then(flights => {
        flights.forEach((flight) => {
          adminContent.innerHTML += `
            <div>
              Flight: ${flight.flightNumber} | Company: ${flight.company} | Source: ${flight.source} | Destination: ${flight.destination} | Seats: ${flight.totalSeats}
            </div>
          `;
        });
      })
      .catch(error => console.error("Error fetching flights:", error));
  });

  // Passenger Dashboard
  document.getElementById("searchFlightsBtn").addEventListener("click", () => {
    passengerContent.innerHTML = `
      <h3>Search Flights</h3>
      <form id="searchForm">
        <label>Source: <input type="text" id="searchSource"></label><br>
        <label>Destination: <input type="text" id="searchDestination"></label><br>
        <button type="submit">Search</button>
      </form>
    `;
    document.getElementById("searchForm").addEventListener("submit", (e) => {
      e.preventDefault();
      const source = document.getElementById("searchSource").value;
      const destination = document.getElementById("searchDestination").value;

      fetch(`http://localhost:5001/api/flights?source=${source}&destination=${destination}`)
        .then(response => response.json())
        .then(flights => {
          passengerContent.innerHTML = `<h3>Search Results for ${source} to ${destination}</h3>`;
          if (flights.length > 0) {
            flights.forEach(flight => {
              passengerContent.innerHTML += `
                <div>
                  Flight: ${flight.flightNumber} | Company: ${flight.company} | Source: ${flight.source} | Destination: ${flight.destination} | Seats: ${flight.availableSeats}
                </div>
              `;
            });
          } else {
            passengerContent.innerHTML += "<p>No flights found.</p>";
          }
        })
        .catch(error => {
          console.error("Error searching flights:", error);
          passengerContent.innerHTML += "<p>Something went wrong. Please try again.</p>";
        });
    });
  });

  document.getElementById("viewReservationsBtn").addEventListener("click", () => {
    passengerContent.innerHTML = "<h3>Your Reservations</h3>";

    // Assume passenger ID is stored somewhere (e.g., in localStorage or sessionStorage)
    const passengerName = "John Doe"; // Replace with the actual passenger name

    fetch(`http://localhost:5001/api/flights/${passengerName}/reservations`)
      .then(response => response.json())
      .then(reservations => {
        if (reservations.length > 0) {
          reservations.forEach(reservation => {
            passengerContent.innerHTML += `
              <div>
                Flight: ${reservation.flightNumber} | Company: ${reservation.company} | Seats Reserved: ${reservation.seatsReserved}
              </div>
            `;
          });
        } else {
          passengerContent.innerHTML += "<p>No reservations found.</p>";
        }
      })
      .catch(error => {
        console.error("Error fetching reservations:", error);
        passengerContent.innerHTML += "<p>Something went wrong. Please try again.</p>";
      });
  });
});
