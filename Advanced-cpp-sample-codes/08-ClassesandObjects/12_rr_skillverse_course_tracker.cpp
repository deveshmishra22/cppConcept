// Capstone exercise sample -- combines everything from classes and access
// control through inheritance, virtual dispatch, abstract classes, and
// overloading in one program. See exercise.html for the full brief.
//
// Use case: RR Skillverse, a platform built to make learning accessible,
// interesting, and available to everyone -- tracking a learner's progress
// in a course, granting extra time when someone needs it, and issuing a
// certificate the moment a course is actually finished.
#include <iostream>
#include <string>
using namespace std;

// Every course format on RR Skillverse is a Course first (inheritance).
class Course {
protected:
    string learnerName;
    string courseTitle;
    int daysExtended = 0;

    // Static members: one shared, platform-wide dashboard for every course object.
    static int totalEnrollments;
    static int totalCertificatesIssued;

public:
    Course(const string& learner, const string& title)
        : learnerName(learner), courseTitle(title) {
        ++totalEnrollments;
        cout << "[enrolled] " << learnerName << " joined \"" << courseTitle << "\"\n";
    }

    // Virtual destructor: safe cleanup through a base pointer/reference.
    virtual ~Course() {
        --totalEnrollments;
        cout << "[exited] " << learnerName << " left \"" << courseTitle << "\"\n";
    }

    // Pure virtual functions: every concrete course must define its own contract (abstract class).
    virtual bool isComplete() const = 0;
    virtual string statusLine() const = 0;
    virtual double weeklyHours() const = 0;

    // Function overloading: two different ways to grant a learner extra time.
    void requestExtraTime(int days) {
        daysExtended += days;
    }
    void requestExtraTime(int days, const string& reason) {
        daysExtended += days;
        cout << "  " << learnerName << " granted " << days << " extra day(s): " << reason << "\n";
    }

    static int enrollmentCount() { return totalEnrollments; }
    static int certificatesIssued() { return totalCertificatesIssued; }

    // Friend function: certificate issuing needs direct access to protected learner/course data.
    friend void issueCertificateIfComplete(const Course& course);

    friend ostream& operator<<(ostream& out, const Course& course) {
        out << course.learnerName << " -> " << course.courseTitle
            << " [" << course.statusLine() << "]";
        return out;
    }

    // Operator overloading: combine two enrollments into one weekly study-hours estimate.
    friend double operator+(const Course& a, const Course& b) {
        return a.weeklyHours() + b.weeklyHours();
    }
};

int Course::totalEnrollments = 0;
int Course::totalCertificatesIssued = 0;

void issueCertificateIfComplete(const Course& course) {
    if (course.isComplete()) {
        ++Course::totalCertificatesIssued;
        cout << "  Certificate issued to " << course.learnerName
             << " for \"" << course.courseTitle << "\"\n";
    } else {
        cout << "  Not yet eligible for a certificate: " << course.learnerName << "\n";
    }
}

class VideoCourse : public Course {
    double percentWatched;
public:
    VideoCourse(const string& learner, const string& title, double percent)
        : Course(learner, title), percentWatched(percent) {}
    bool isComplete() const override { return percentWatched >= 90.0; }
    string statusLine() const override {
        return "self-paced video, " + to_string((int)percentWatched) + "% watched";
    }
    double weeklyHours() const override { return 2.5; }
};

class LiveWorkshop : public Course {
    int sessionsAttended;
    int totalSessions;
public:
    LiveWorkshop(const string& learner, const string& title, int attended, int total)
        : Course(learner, title), sessionsAttended(attended), totalSessions(total) {}
    bool isComplete() const override { return sessionsAttended >= totalSessions; }
    string statusLine() const override {
        return "live workshop, " + to_string(sessionsAttended) + "/" +
               to_string(totalSessions) + " sessions attended";
    }
    double weeklyHours() const override { return totalSessions * 1.5; } // 1.5 hrs per session
};

int main() {
    VideoCourse meera("Meera", "C++ Foundations", 92.0);
    LiveWorkshop dev("Dev", "C++ Mentorship Circle", 3, 4);

    cout << meera << "\n";
    cout << dev << "\n";

    dev.requestExtraTime(3, "exam week conflict"); // second overload

    cout << "Combined weekly study hours for Meera+Dev: " << (meera + dev) << "\n";

    issueCertificateIfComplete(meera);
    issueCertificateIfComplete(dev);

    cout << "Active enrollments: " << Course::enrollmentCount() << "\n";
    cout << "Certificates issued so far: " << Course::certificatesIssued() << "\n";
}
