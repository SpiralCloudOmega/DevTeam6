# .NET 8 Web API Starter Template

A complete .NET 8 Web API starter template with OpenAPI/Swagger, CORS, and sample controllers pre-configured.

## Features

- 🚀 **.NET 8** - Latest .NET with minimal APIs
- 📚 **OpenAPI/Swagger** - Interactive API documentation
- 🔄 **CORS** - Cross-Origin Resource Sharing enabled
- 🎯 **Controllers** - Sample MVC controllers
- 📝 **Logging** - Built-in structured logging
- ⚙️ **Configuration** - appsettings.json configuration
- 🔧 **Dependency Injection** - Built-in DI container

## Getting Started

### Prerequisites

- .NET 8 SDK

### Running the API

```bash
dotnet run
```

The API will be available at:
- HTTP: `http://localhost:5000`
- HTTPS: `https://localhost:5001`
- Swagger UI: `https://localhost:5001/swagger`

### Build

```bash
dotnet build
```

### Restore Dependencies

```bash
dotnet restore
```

### Run Tests (when added)

```bash
dotnet test
```

## Project Structure

```
dotnet8-api/
├── Controllers/              # API controllers
│   └── WeatherForecastController.cs
├── Models/                   # Data models
│   └── WeatherForecast.cs
├── Properties/               # Launch settings
│   └── launchSettings.json
├── Services/                 # Business logic services
├── Program.cs                # Application entry point
├── appsettings.json          # Configuration
├── dotnet8-api.csproj        # Project file
└── dotnet8-api.sln           # Solution file
```

## API Endpoints

### Health Check
```
GET /api/health
```
Returns API health status.

### Hello World
```
GET /api/hello?name=YourName
```
Simple greeting endpoint.

### Weather Forecast
```
GET /api/weatherforecast
```
Returns a 5-day weather forecast.

```
GET /api/weatherforecast/{days}
```
Returns weather forecast for a specific day.

## Development

### Adding a New Controller

1. Create a new file in `Controllers/` directory
2. Inherit from `ControllerBase`
3. Add the `[ApiController]` and `[Route]` attributes
4. Implement your endpoints

Example:
```csharp
[ApiController]
[Route("api/[controller]")]
public class MyController : ControllerBase
{
    [HttpGet]
    public IActionResult Get()
    {
        return Ok(new { message = "Hello" });
    }
}
```

### Adding a New Service

1. Create interface in `Services/` directory
2. Create implementation class
3. Register in `Program.cs`:
```csharp
builder.Services.AddScoped<IMyService, MyService>();
```

## Configuration

Configuration is stored in `appsettings.json` and `appsettings.Development.json`.

Access configuration in your code:
```csharp
var myValue = builder.Configuration["MyKey"];
```

## Technologies

- [.NET 8](https://dotnet.microsoft.com/) - Cross-platform framework
- [ASP.NET Core](https://docs.microsoft.com/aspnet/core/) - Web framework
- [Swashbuckle](https://github.com/domaindrivendev/Swashbuckle.AspNetCore) - OpenAPI/Swagger tools
- [Entity Framework Core](https://docs.microsoft.com/ef/core/) - (Add as needed)

## Customization

This template is designed to be a starting point. Feel free to:

- Add Entity Framework Core for database access
- Add authentication with JWT or OAuth
- Add AutoMapper for object mapping
- Add FluentValidation for request validation
- Add MediatR for CQRS pattern
- Add Redis for caching
- Add RabbitMQ or Azure Service Bus for messaging

## Learn More

- [.NET Documentation](https://docs.microsoft.com/dotnet/)
- [ASP.NET Core Documentation](https://docs.microsoft.com/aspnet/core/)
- [Minimal APIs](https://docs.microsoft.com/aspnet/core/fundamentals/minimal-apis)
