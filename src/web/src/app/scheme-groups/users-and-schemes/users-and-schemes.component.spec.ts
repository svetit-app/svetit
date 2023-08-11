import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UsersAndSchemesComponent } from './users-and-schemes.component';

describe('UsersAndDevicesComponent', () => {
  let component: UsersAndSchemesComponent;
  let fixture: ComponentFixture<UsersAndSchemesComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UsersAndSchemesComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(UsersAndSchemesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
