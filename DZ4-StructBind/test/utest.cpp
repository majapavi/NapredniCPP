#include "../src/log.h"
#include "gtest/gtest.h"

TEST(Log, Konstruktor)
{
    std::array<char, 3> code{ 'E','U','R' };
    Log Log1("pero", 12, code, "Prva poruka");

    EXPECT_EQ(Log1.sender(), std::string("pero"));
    EXPECT_EQ(Log1.priority(), 12);
    EXPECT_EQ(Log1.message(), std::string("Prva poruka"));
    EXPECT_EQ(Log1.code(), code);

    //    Log1.sender() = std::string("Maja");
    Log1.priority() = 999;
    //    Log1.message() = std::string("Hello World!");
    //    Log1.code() = std::array<char, 3>{ 'H','R','K' };

    EXPECT_EQ(Log1.priority(), 999);
}

TEST(Log, binding1)
{
    std::string name = "Pero";
    std::array<char, 3> cde{ 'E','U','R' };
    std::string mesg = "Druga poruka";
    Log Log1(name, 13, cde, mesg);

    auto [sender, priority, code, message] = Log1;

    EXPECT_EQ(sender, name);
    EXPECT_EQ(priority, 13);
    EXPECT_EQ(message, mesg);
    EXPECT_EQ(code, cde);

    // Struct bind vraca kopiju klase pa su promjene novih varijabli i varijabli clanica klase medusobno neovisne
    priority = 100;
    EXPECT_EQ(Log1.priority(), 13);

    //sender = std::string("Maja");
    //Log1.sender() = std::string("Maja");
}

TEST(Log, binding2)
{
    std::string name = "Pero";
    std::array<char, 3> cde{ 'E','U','R' };
    std::string mesg = "Druga poruka";
    Log Log1(name, 13, cde, mesg);

    auto& [sender, priority, code, message] = Log1;

    EXPECT_EQ(sender, name);
    EXPECT_EQ(priority, 13);
    EXPECT_EQ(message, mesg);
    EXPECT_EQ(code, cde);

    // Struct bind vraca lijevu referencu na nasu klasu pa promjena novih varijabli mijenja vrijednosti u klasi i obrnuto
    // s tim da je, implementacijom samo konstantnih get funkcija za neke varijable clanice,
    // napravljeno da se izvan klase mijenjati moze samo prioritet logirane poruke
    priority = 10;

    EXPECT_EQ(Log1.priority(), priority);
    Log1.priority() = 12;
    EXPECT_EQ(Log1.priority(), priority);

    //sender = std::string("Maja");
    //Log1.sender() = std::string("Maja");
}

TEST(Log, binding3)
{
    std::string name = "Pero";
    std::array<char, 3> cde{ 'E','U','R' };
    std::string mesg = "Druga poruka";
    Log Log1(name, 13, cde, mesg);

    auto const& [sender, priority, code, message] = Log1;

    // Struct bind vraca konstantnu lijevu referencu na klasu pa se preko novih "varijabli" ne mogu mijenjati vrijednosti iz klase
    // no promjene ucinjene nad elementima klase ce biti vidljive kroz iste te varijable
    EXPECT_EQ(sender, name);
    EXPECT_EQ(priority, 13);
    EXPECT_EQ(message, mesg);
    EXPECT_EQ(code, cde);

    Log1.priority() = 12;
    EXPECT_EQ(Log1.priority(), priority);

    //sender = std::string("Maja");
    //Log1.sender() = std::string("Maja");
}

TEST(Log, binding4)
{
    std::string name = "Pero";
    std::array<char, 3> cde{ 'E','U','R' };
    std::string mesg = "Druga poruka";
    Log Log1(name, 13, cde, mesg);

    auto&& [sender, priority, code, message] = std::move(Log1);

    EXPECT_EQ(sender, name);
    EXPECT_EQ(priority, 13);
    EXPECT_EQ(message, mesg);
    EXPECT_EQ(code, cde);

    priority = 10;

    EXPECT_EQ(Log1.priority(), priority);
    Log1.priority() = 12;
    EXPECT_EQ(Log1.priority(), priority);

    //sender = std::string("Maja");
    //Log1.sender() = std::string("Maja");
}

TEST(Log, binding5)
{
    std::string name = "Pero";
    std::array<char, 3> cde{ 'E','U','R' };
    std::string mesg = "Druga poruka";
    Log Log1(name, 13, cde, mesg);

    auto [sender, priority, code, message] = std::move(Log1);

    EXPECT_EQ(sender, name);
    EXPECT_EQ(priority, 13);
    EXPECT_EQ(message, mesg);
    EXPECT_EQ(code, cde);

    priority = 10;
    EXPECT_NE(Log1.priority(), priority);
    Log1.priority() = 12;
    EXPECT_NE(Log1.priority(), priority);

    //sender = std::string("Maja");
    //Log1.sender() = std::string("Maja");
}