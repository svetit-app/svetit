import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { UserTestComponent } from './user.component';

describe('UserTestComponent', () => {
  let component: UserTestComponent;
  let fixture: ComponentFixture<UserTestComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ UserTestComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(UserTestComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
