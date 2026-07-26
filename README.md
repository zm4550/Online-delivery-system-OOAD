# Online Delivery System

A console-based Online Delivery System built in C++, designed using Object-Oriented Analysis and Design (OOAD). Includes UML class and activity diagrams for the design and implementation process.

## Overview

The system models a shopping and delivery platform with several user roles:

- **Customer** — browse shops/items, place orders
- **Shopkeeper** — manage shop and items
- **Delivery Boy** — handle deliveries
- **Moderator** — system administrator role

## Design

- [Class Diagram](class-diagram.pdf) — models the core classes (`User`, `Customer`, `Shopkeeper`, `Shop`, `Item`, `DeliveryBoy`, `Transaction`, `Moderator`) and their relationships
- [Activity Diagram](activity-diagram.pdf) — models the flow of key processes (e.g. placing an order, delivery)

## Tech Stack

- **Language:** C++
- **Paradigm:** Object-Oriented Programming (OOP), designed with OOAD principles

## Getting Started

### Prerequisites

- A C++ compiler (e.g. g++, MinGW, or any IDE with C++ support like Code::Blocks or Visual Studio)

### Compile & Run

```bash
g++ "Online Delivery System1.cpp" -o delivery_system
./delivery_system
```

On Windows (with MinGW):
```bash
g++ "Online Delivery System1.cpp" -o delivery_system.exe
delivery_system.exe
```

## Features

- User registration and role-based access (Customer, Shopkeeper, Delivery Boy, Moderator)
- Shop and item management
- Order placement and delivery slot scheduling
- Transaction tracking
- Date-based delivery scheduling

## License

This project is open source and available under the [MIT License](LICENSE).
